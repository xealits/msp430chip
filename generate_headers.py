'''
Generate device header files from an HTML datasheet document.
'''

from bs4 import BeautifulSoup
import csv
from collections import defaultdict
from pathlib import Path
from sys import argv
from pprint import pprint

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

def parse_field(bs_elem):
    name = bs_elem.select_one(":scope dfn").text.strip()
    offset = int(bs_elem.select_one(":scope span.offset").text.strip())
    width = int(bs_elem.select_one(":scope span.width").text.strip())
    info = {"offset": offset, "width": width}
    return name, info

def parse_register(bs_elem):
    name = bs_elem.select_one(":scope dfn:not(:scope details dfn)").text.strip()
    width = 32
    cpp_type = {32: "uint32_t"}[width]
    reg_info = {"width": width, "cpp_type": cpp_type}

    # reg fields
    reg_fields = {}
    reg_info["fields"] = reg_fields

    substr = bs_elem.select_one(":scope details")
    bs_fields = []
    if substr:
        bs_fields = substr.select(":scope li.field") or []

    for field_elem in bs_fields:
        field_name, field_info = parse_field(field_elem)
        assert field_name not in reg_fields
        reg_fields[field_name] = field_info

    return name, reg_info

def parse_device_template(bs_elem, known_templates={}):
    dev_name = bs_elem.select_one(":scope dfn:not(:scope details dfn)").text.strip()
    if dev_name in device_templates:
        raise Exception(f"Device template {dev_name} already exists")

    templ = {}
    known_templates[dev_name] = templ

    substr = bs_elem.select_one(":scope details")

    # parse registers
    regs = substr.select(":scope li.register:not(:scope details li.register)")
    dev_registers = {}
    templ["registers"] = dev_registers
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