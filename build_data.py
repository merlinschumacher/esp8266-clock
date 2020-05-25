Import("env")

def inplace_change(filename, old_string, new_string):
    # Safely read the input filename using 'with'
    with open(filename) as f:
        s = f.read()
        if old_string not in s:
            print('"{old_string}" not found in {filename}.'.format(**locals()))
            return

    # Safely write the changed content, if found in the file

print("converting web files to headers");


source_files = ["index.html", "main.js", "timezones.json", "water.css"]
target_files = ["index_html.h", "main_js.h", "timezones_json.h", "water_css.h"]
i = 0
while i < len(source_files):
    print(i)
    print(source_files[i])
    env.Execute("gzip -c -9 web/"+source_files[i]+"> web/"+source_files[i]+".gz")
    env.Execute("xxd -i web/"+source_files[i]+".gz src/"+target_files[i])
    with open("src/"+target_files[i], 'r+') as f:
        s = f.read()
        s = s.replace("unsigned", "const")
        s = s.replace("] =", "] PROGMEM =")
        f.write(s)
    env.Execute("rm web/"+source_files[i]+".gz")
    i = i+1
    # env.Execute("xxd -i web/index.html.gz src/index_html.h")
    # env.Execute("xxd -i web/main.js.gz src/main_js.h")
    # env.Execute("xxd -i web/timezones.json.gz src/timezones_json.h")
    # env.Execute("xxd -i web/water.css.gz src/water_css.h")


#inplace_change("web/index_html.h", "unsigned", "const")
#inplace_change("web/", "unsigned", "const")