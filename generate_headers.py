'''
Generate device header files from an HTML datasheet document.
'''

from bs4 import BeautifulSoup
import csv
from collections import defaultdict
from pathlib import Path
from sys import argv

fname = Path(argv[1])

with open(fname, 'r') as f:
    example_s = f.read()
    
soup = BeautifulSoup(example_s, 'html.parser')
#soup.select(".perf_records")[0].select("summary")[0]
#soup.select(".perf_records")[0].find("summary").find("var").get_text(strip=True)
#'testing X'

def top_most(tag, class_=None):
    def matcher(element):
        if class_ is not None:
            same_parents = element.find_parents(tag, class_=class_)
        else:
            same_parents = element.find_parents(tag)

        element_is_topmost = len(same_parents) == 0
        #if len(same_parents) > 0:
        #    return False
        #return True
        return element_is_topmost

    return matcher

def find_topmost(soup, tag, class_ = None):
    if class_ is not None:
        all_els = soup.find_all(tag, class_=class_)
        found_els = [el for el in all_els if not el.find_parent(tag, class_=class_)]

    else:
        all_els = soup.find_all(tag)
        found_els = [el for el in all_els if not el.find_parent(tag)]

    return found_els

#print(len(soup.select("div.device")))
#print(len(soup.find_all("div")))
#print(len(soup.find_all(top_most("div", "device"), "div", class_="device")))
# top devices are divs
# so that HTML does not require its text rules
# which are applied to sections and everything
devices = find_topmost(soup, "div", "device")
print(len(devices))

# get just the registers
for dev in devices:
    # select starting from the current node (:scope)
    # dfn tags that are not part of details
    dev_name = dev.select_one(":scope dfn:not(:scope details dfn)").text.strip()
    print(dev_name)

    substr = dev.select_one(":scope details")

    # then regs are in details
    # and they are always listed as li of ul
    regs = substr.select(":scope li.register:not(:scope details li.register)")

    # this kind of works:
    #reg_names = [reg.select(":scope > dfn") for reg in regs]
    # but I want to keep the any-descendants flexibility
    reg_names = [reg.select(":scope dfn:not(:scope details dfn)") for reg in regs]
    print(len(regs), reg_names)

    subdevs = substr.select(":scope li.device:not(:scope details li.device)")
    dev_names = [dev.select(":scope dfn:not(:scope details dfn)") for dev in subdevs]
    print(len(subdevs), dev_names)
