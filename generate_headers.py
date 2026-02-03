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
devices = find_topmost(soup, "div", "device")
print(len(devices))

# get just the registers
for dev in devices:
    # markdown wraps with the <p>s....
    # this is it:
    print(dev.select_one("dfn:not(details dfn)").text.strip())