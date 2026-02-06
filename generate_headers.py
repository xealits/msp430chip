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

reg_field_template = "using {name} = BitField<decltype({reg_ref}), {reg_ref}, {offset}, {width}>;"
reg_template = "struct {name} : public Register<decltype({reg_ref}), {reg_ref}> {body};"
device_template = """
template<{reg_declarations}>
struct {name} {{
  {name}() = delete;

  {reg_definitions}
}};
"""

@dataclass
class RegField:
    name: str
    offset: int
    width: int

    def to_cpp(self, reg_ref):
        return reg_field_template.format(name=self.name, reg_ref=reg_ref, offset=self.offset, width=self.width)

@dataclass
class Register:
    name: str
    width: int = 32
    fields: dict = field(default_factory=dict)

    def __getattr__(self, name):
        known_types = {32: "uint32_t"}
        if name == "cpp_type":
            return known_types.get(self.width)

        raise AttributeError(f"'Register' object has no attribute {name}")

    def to_cpp(self, reg_ref):
        fields_str = '\n'.join("  " + f.to_cpp(reg_ref) for f in self.fields.values())
        if fields_str:
            body = f"{{\n{fields_str}\n}}"
        else:
            body = "{}"
        return reg_template.format(name=self.name, reg_ref=reg_ref, body=body)

# TODO: there are 2 types of devices: unique and templates
@dataclass
class Device:
    name: str
    registers: dict = field(default_factory=dict)

    def to_cpp(self):
        reg_decls = []
        reg_defs = []
        for reg in self.registers.values():
            reg_ref = reg.name + "_t"
            cpp_type = reg.cpp_type
            reg_decl = f"volatile {cpp_type}& {reg_ref}"

            reg_decls.append(reg_decl)
            reg_defs.append(reg.to_cpp(reg_ref))

        reg_decls = "\n".join(reg_decls)
        reg_defs = "\n".join(reg_defs)

        return device_template.format(name=self.name, reg_declarations=reg_decls, reg_definitions=reg_defs)

def parse_field(bs_elem):
    name = bs_elem.select_one(":scope dfn").text.strip()
    offset = int(bs_elem.select_one(":scope span.offset").text.strip())
    width = int(bs_elem.select_one(":scope span.width").text.strip())
    #info = {"offset": offset, "width": width}
    field = RegField(name, offset, width)
    return name, field

def parse_register(bs_elem):
    name = bs_elem.select_one(":scope dfn:not(:scope details dfn)").text.strip()
    width = 32
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

def parse_device_template(bs_elem, known_templates={}):
    dev_name = bs_elem.select_one(":scope dfn:not(:scope details dfn)").text.strip()
    if dev_name in device_templates:
        raise Exception(f"Device template {dev_name} already exists")

    dev_registers = {}
    templ = Device(dev_name, dev_registers)
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
        parse_device_template(subdev, known_templates)

device_templates = {}

devices = find_topmost(soup, "div", "device_template")
print("Number of top-level devices:", len(devices))

# get just the registers
for dev in devices:
    parse_device_template(dev, device_templates)

pprint(device_templates)

for reg in device_templates["TimerA"].registers.values():
    reg_ref = reg.name + "_t"
    print(reg.to_cpp(reg_ref))

print(device_templates["TimerA"].to_cpp())