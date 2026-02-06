'''
Generate device header files from an HTML datasheet document.
'''

from bs4 import BeautifulSoup
import csv
from collections import defaultdict
from pathlib import Path
from sys import argv
from pprint import pprint
from dataclasses import dataclass, field
from textwrap import indent

fname = Path(argv[1])

with open(fname, 'r') as f:
    example_s = f.read()
    
soup = BeautifulSoup(example_s, 'html.parser')

def find_topmost(soup, tag, class_ = None):
    if class_ is not None:
        all_els = soup.find_all(tag, class_=class_)
        found_els = [el for el in all_els if not el.find_parent(tag, class_=class_)]

    else:
        all_els = soup.find_all(tag)
        found_els = [el for el in all_els if not el.find_parent(tag)]

    return found_els

templ_space = " " * len("template<")
reg_field_template = "using {name} = BitField<decltype({reg_ref}), {reg_ref}, {offset}, {width}>;"
reg_field_options_template = "constexpr static typename {name}::OPT{opts};"

reg_template = "struct {name} : public Register<decltype({reg_ref}), {reg_ref}> {body};"

device_template = """
template<{reg_declarations}>
struct {name} {{
  {name}() = delete;

{device_defs}
}};
"""

subdev_def_template = """
template<unsigned dev_i>
using {devname} = typename {dev_ref_t}::template DevByIndex<dev_i>;"""

subdev_decl_template = """
template<{dev_refs}>
struct {name}Params {{}};

template<typename {name}Params_t>
struct {name}Template;

template<{dev_refs}>
struct {name}Template<{name}Params<{dev_refs_used}>> {{
  {name}Template() = delete;

{dev_defs}
}};
"""

known_reg_types_cpp = {16: "unsigned int", 32: "unsigned long"}

@dataclass
class RegFieldOptValue:
    name: str
    bits: int

@dataclass
class RegField:
    name: str
    offset: int
    width: int
    option_values: list = field(default_factory=list)

    def to_cpp(self, reg_ref, indentation=0):
        contents = reg_field_template.format(name=self.name, reg_ref=reg_ref, offset=self.offset, width=self.width)

        if self.option_values:
            opts = "\n  " + ",\n  ".join(f"{opt.name}{{{opt.bits}}}" for opt in self.option_values)
            opts_defs = reg_field_options_template.format(name=self.name, opts=opts)
            contents += "\n" + opts_defs

        return indent(contents, "  "*indentation)

@dataclass
class Register:
    name: str
    width: int = 16
    fields: dict = field(default_factory=dict)

    def __getattr__(self, name):
        if name == "cpp_type":
            return known_reg_types_cpp[self.width]

        raise AttributeError(f"'Register' object has no attribute {name}")

    def to_cpp(self, reg_ref, indentation=0):
        fields_str = '\n'.join(f.to_cpp(reg_ref, 1) for f in self.fields.values())
        if fields_str:
            body = f"{{\n{fields_str}\n}}"
        else:
            body = "{}"

        contents = reg_template.format(name=self.name, reg_ref=reg_ref, body=body)
        return indent(contents, "  "*indentation)

# TODO: there are 2 types of devices: unique and templates
# unique is just a bunch of registers that are logically connected
# if such a case happens, it could be useful to separate them
# into an anonymous struct in Cpp
# templates cover all cases when there are multiple identical devices
# that are accessed by a template <index> parameter
@dataclass
class Device:
    name: str
    registers: dict = field(default_factory=dict)
    subdev_names: list = field(default_factory=list)

    def _reg_decls_n_defs(self, indentation=0):
        reg_decls = []
        reg_defs = []
        for reg in self.registers.values():
            reg_ref = reg.name + "_t"
            cpp_type = reg.cpp_type
            reg_decl = f"volatile {cpp_type}& {reg_ref}"

            reg_decls.append(reg_decl)
            reg_defs.append(reg.to_cpp(reg_ref, indentation+1))

        reg_decls = f",\n{templ_space}".join(reg_decls)
        reg_defs = "\n\n".join(reg_defs)

        return reg_decls, reg_defs

    def to_cpp(self, indentation=0):
        reg_decls, reg_defs = self._reg_decls_n_defs()
        all_decls = reg_decls
        all_defs = reg_defs

        dev_decls = []
        dev_defs = []
        for devname in self.subdev_names:
            dev_ref_t = devname + "_t"
            dev_decls.append(f"typename {dev_ref_t}")
            dev_def = subdev_def_template.format(devname=devname, dev_ref_t=dev_ref_t)
            dev_defs.append(indent(dev_def, "  "))

        if dev_decls:
            all_decls += f",\n{templ_space}" + f",\n{templ_space}".join(dev_decls)
            all_defs += "\n" + "\n\n".join(dev_defs)

        contents = device_template.format(name=self.name, reg_declarations=all_decls, device_defs=all_defs)
        return indent(contents, "  "*indentation)

@dataclass
class SubDevice(Device):
    name: str
    registers: dict = field(default_factory=dict)
    subdev_names: list = field(default_factory=list)

    def to_cpp(self, indentation=0):
        #reg_params = []
        #for reg in self.registers.values():
        #    param = f"{reg.cpp_type}& {reg.name}_t"
        #    reg_params.append(param)
        reg_decls, reg_defs = self._reg_decls_n_defs(indentation)

        dev_refs = reg_decls
        dev_defs = reg_defs

        dev_refs_used = [reg.name + "_t" for reg in self.registers.values()]
        dev_refs_used = ", ".join(dev_refs_used)

        # TODO: add sub-devices

        cpp = subdev_decl_template.format(name=self.name, dev_refs=dev_refs, dev_refs_used=dev_refs_used, dev_defs=dev_defs)
        return cpp

def parse_field(bs_elem):
    name = bs_elem.select_one(":scope dfn").text.strip()
    offset = int(bs_elem.select_one(":scope span.offset").text.strip())
    width = int(bs_elem.select_one(":scope span.width").text.strip())
    #info = {"offset": offset, "width": width}

    opt_dict = {}
    for opt in bs_elem.select(":scope > details > data.value_option"):
        opt_name = opt.text.strip()
        value = int(opt["value"])
        assert opt_name not in opt_dict
        opt_dict[opt_name] = value

    opt_values = []
    for opt_name, value in sorted(opt_dict.items(), key=lambda it: it[1]):
        opt_values.append(RegFieldOptValue(opt_name, value))

    field = RegField(name, offset, width, opt_values)
    return name, field

def parse_register(bs_elem):
    name = bs_elem.select_one(":scope dfn:not(:scope details dfn)").text.strip()
    width = 16  # TODO: add to data-sheet
    #cpp_type = {32: "uint32_t"}[width]
    #reg_info = {"width": width, "cpp_type": cpp_type}

    # reg fields
    reg_fields = {}
    reg = Register(name, width, reg_fields)

    substr = bs_elem.select_one(":scope details")
    bs_fields = []
    if substr:
        bs_fields = substr.select(":scope li.field") or []

    for field_elem in bs_fields:
        field_name, field_info = parse_field(field_elem)
        assert field_name not in reg_fields
        reg_fields[field_name] = field_info

    return name, reg

def parse_device_template(bs_elem, known_templates={}, toplevel=False):
    dev_name = bs_elem.select_one(":scope dfn:not(:scope details dfn)").text.strip()
    if dev_name in device_templates:
        raise Exception(f"Device template {dev_name} already exists")

    dev_registers = {}
    dev_subdev_names = []
    if toplevel:
        templ = Device(dev_name, dev_registers, dev_subdev_names)
    else:
        templ = SubDevice(dev_name, dev_registers, dev_subdev_names)
    known_templates[dev_name] = templ

    substr = bs_elem.select_one(":scope details")

    # parse registers
    regs = substr.select(":scope li.register:not(:scope details li.register)")
    for reg in regs:
        name, info = parse_register(reg)
        assert name not in dev_registers
        dev_registers[name] = info

    # TODO: parse unique sub-devices

    # parse sub-device templates
    # "templates" are sub-devices that can appear in variable number
    subdev_templates = substr.select(":scope li.device_template:not(:scope details li.device_template)")
    for subdev in subdev_templates:
        subdev_name = parse_device_template(subdev, known_templates)
        dev_subdev_names.append(subdev_name)

    return dev_name

device_templates = {}

devices = find_topmost(soup, "div", "device_template")
#print("Number of top-level devices:", len(devices))

# the top level devices are divs
# the subdevices are the usual li inside the divs
for dev in devices:
    parse_device_template(dev, device_templates, True)

#pprint(device_templates)

#for reg in device_templates["TimerA"].registers.values():
#    reg_ref = reg.name + "_t"
#    print(reg.to_cpp(reg_ref))

devices_toplevel = ""
devices_subdevices = ""
for devname, dev in device_templates.items():
    if isinstance(dev, SubDevice):
        devices_subdevices += dev.to_cpp()
    else:
        devices_toplevel += dev.to_cpp()

#print(devices_subdevices)
#print(devices_toplevel)

source_template = """
#pragma once

#include <cstdint>
#include "devpacks.hpp"
#include "bitlogic.hpp"

namespace devices {{
using bitlogic::BitField;
using bitlogic::Register;

{sources}
}};
"""

all_sources = devices_subdevices + devices_toplevel
print(source_template.format(sources=all_sources))
