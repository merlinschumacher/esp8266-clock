Import("env")
import os

def inplace_change(filename, old_string, new_string):
    # Safely read the input filename using 'with'
    with open(filename) as f:
        s = f.read()
        if old_string not in s:
            print('"{old_string}" not found in {filename}.'.format(**locals()))
            return

    # Safely write the changed content, if found in the file

print("converting web files to headers");

env.Execute("gzip -k web/*")

source_files = ['index.html.gz', 'main.js.gz', 'timezones.json.gz', 'water.css.gz']
target_files = ['index_html.h', 'main_js.h', 'timezones_json.h', 'water_css.h']

for x in range(4):
    print("converting file ") + source_files['']
    env.Execute("xxd -i web/"+source_files[x]+" src/"+target_files[x])
    inplace_change("web/"+target_files[x], "unsigned", "const")
    inplace_change("web/"+target_files[x], "] =", "] PROGMEM =")
    with open(target_files[x], 'w') as f:
        s = f.read()
        s = s.replace("unsigned", "const")
        s = s.replace("] =", "] PROGMEM =")
        f.write(s)
    # env.Execute("xxd -i web/index.html.gz src/index_html.h")
    # env.Execute("xxd -i web/main.js.gz src/main_js.h")
    # env.Execute("xxd -i web/timezones.json.gz src/timezones_json.h")
    # env.Execute("xxd -i web/water.css.gz src/water_css.h")

env.Execute("rm web/*.gz")

#inplace_change("web/index_html.h", "unsigned", "const")
#inplace_change("web/", "unsigned", "const")