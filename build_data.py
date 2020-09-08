Import("env")

build_tag = env['PIOENV']
print("converting web files to headers");
source_files = ["index.html", "main.js", "timezones.json", "water.css"]
target_files = ["index_html.h", "main_js.h", "timezones_json.h", "water_css.h"]
i = 0
while i < len(source_files):
    env.Execute("gzip -c -9 web/"+source_files[i]+"> web/"+source_files[i]+".gz")
    env.Execute("xxd -i web/"+source_files[i]+".gz include/"+target_files[i])
    with open("include/"+target_files[i], 'r+') as f:
        s = f.read()
        s = s.replace("unsigned char", "const char")
        s = s.replace("web_", "")
        s = s.replace("] =", "] PROGMEM =")
        f.truncate(0)
        f.seek(0)
        f.write(s)
        f.close()
    env.Execute("rm web/"+source_files[i]+".gz")
    i = i+1

print("changing program filename to ") 
print( PROGNAME="firmware_%s" % build_tag )
env.Replace(PROGNAME="firmware_%s" % build_tag)