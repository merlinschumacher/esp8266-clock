Import("env")

build_tag = env['PIOENV']
env.Execute("cd web; gulp build; cd ..")
print("converting web files to headers");
source_files = ["index-en.html", "index-de.html", "scripts.js", "styles.css"]
target_files = ["index_en_html.hpp", "index_de_html.hpp", "scripts_js.hpp", "styles_css.hpp"]
i = 0
while i < len(source_files):
    env.Execute("gzip -c -9 web/dist/"+source_files[i]+"> web/dist/"+source_files[i]+".gz")
    env.Execute("cd web/dist; xxd -i "+source_files[i]+".gz ../../include/"+target_files[i])
    with open("include/"+target_files[i], 'r+') as f:
        s = f.read()
        s = s.replace("unsigned char", "const char")
        s = s.replace("web_", "")
        s = s.replace("] =", "] PROGMEM =")
        f.truncate(0)
        f.seek(0)
        f.write(s)
        f.close()
    env.Execute("rm web/dist/"+source_files[i]+".gz")
    i = i+1


print("changing program filename ") 
env.Replace(PROGNAME="firmware_%s" % build_tag)