function saveConfig(e=!1){document.getElementById("configform").checkValidity()&&(conf=app.models.config,conf.saveData=e,fetch("data.json",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify(conf)}).then(e=>e.json()).then(e=>{console.log("Success:",e)}).catch(e=>{console.error("Error:",e),alert("Failed to save settings!")}))}async function loadConfig(){return(await fetch("/data.json")).json()}async function getVersion(){return await fetch("version").then((function(e){if(e.ok)return e.text();throw new Error("Failed to load version!")})).then((function(e){return e}))}async function getTime(){return await fetch("time").then((function(e){if(e.ok)return e.text();throw new Error("Failed to load current time!")})).then((function(e){return e}))}const languages=[{code:"en",name:"English",icon:"🇬🇧"},{code:"de",name:"Deutsch",icon:"🇩🇪"}],loadLanguage=function(){saveConfig()},toggleResetModal=function(){return document.getElementById("modalreset").classList.toggle("active"),!1},toggleFirmwareModal=function(){return document.getElementById("modalfirmware").classList.toggle("active"),!1};function switchTab(e){const i=e.getAttribute("href").substring(1),t=document.querySelectorAll(".contenttab");document.querySelectorAll("li.tab-item").forEach(e=>{e.id.includes(i)?e.classList.add("active"):e.classList.remove("active")}),t.forEach(e=>{e.id==i?e.style.display="unset":e.style.display="none"})}function initTabs(){document.getElementById("colorconfig").style.display="unset";document.querySelectorAll(".tab-item > a").forEach(e=>{e.addEventListener("click",(function(e){return switchTab(e.target),!1}),!1)})}const timezones=["Africa/Abidjan","Africa/Accra","Africa/Algiers","Africa/Bissau","Africa/Cairo","Africa/Casablanca","Africa/Ceuta","Africa/El_Aaiun","Africa/Johannesburg","Africa/Juba","Africa/Khartoum","Africa/Lagos","Africa/Maputo","Africa/Monrovia","Africa/Nairobi","Africa/Ndjamena","Africa/Sao_Tome","Africa/Tripoli","Africa/Tunis","Africa/Windhoek","America/Adak","America/Anchorage","America/Araguaina","America/Argentina/Buenos_Aires","America/Argentina/Catamarca","America/Argentina/Cordoba","America/Argentina/Jujuy","America/Argentina/La_Rioja","America/Argentina/Mendoza","America/Argentina/Rio_Gallegos","America/Argentina/Salta","America/Argentina/San_Juan","America/Argentina/San_Luis","America/Argentina/Tucuman","America/Argentina/Ushuaia","America/Asuncion","America/Atikokan","America/Bahia","America/Bahia_Banderas","America/Barbados","America/Belem","America/Belize","America/Blanc-Sablon","America/Boa_Vista","America/Bogota","America/Boise","America/Cambridge_Bay","America/Campo_Grande","America/Cancun","America/Caracas","America/Cayenne","America/Chicago","America/Chihuahua","America/Costa_Rica","America/Creston","America/Cuiaba","America/Curacao","America/Danmarkshavn","America/Dawson","America/Dawson_Creek","America/Denver","America/Detroit","America/Edmonton","America/Eirunepe","America/El_Salvador","America/Fort_Nelson","America/Fortaleza","America/Glace_Bay","America/Goose_Bay","America/Grand_Turk","America/Guatemala","America/Guayaquil","America/Guyana","America/Halifax","America/Havana","America/Hermosillo","America/Indiana/Indianapolis","America/Indiana/Knox","America/Indiana/Marengo","America/Indiana/Petersburg","America/Indiana/Tell_City","America/Indiana/Vevay","America/Indiana/Vincennes","America/Indiana/Winamac","America/Inuvik","America/Iqaluit","America/Jamaica","America/Juneau","America/Kentucky/Louisville","America/Kentucky/Monticello","America/La_Paz","America/Lima","America/Los_Angeles","America/Maceio","America/Managua","America/Manaus","America/Martinique","America/Matamoros","America/Mazatlan","America/Menominee","America/Merida","America/Metlakatla","America/Mexico_City","America/Miquelon","America/Moncton","America/Monterrey","America/Montevideo","America/Nassau","America/New_York","America/Nipigon","America/Nome","America/Noronha","America/North_Dakota/Beulah","America/North_Dakota/Center","America/North_Dakota/New_Salem","America/Nuuk","America/Ojinaga","America/Panama","America/Pangnirtung","America/Paramaribo","America/Phoenix","America/Port_of_Spain","America/Port-au-Prince","America/Porto_Velho","America/Puerto_Rico","America/Punta_Arenas","America/Rainy_River","America/Rankin_Inlet","America/Recife","America/Regina","America/Resolute","America/Rio_Branco","America/Santarem","America/Santiago","America/Santo_Domingo","America/Sao_Paulo","America/Scoresbysund","America/Sitka","America/St_Johns","America/Swift_Current","America/Tegucigalpa","America/Thule","America/Thunder_Bay","America/Tijuana","America/Toronto","America/Vancouver","America/Whitehorse","America/Winnipeg","America/Yakutat","America/Yellowknife","Antarctica/Casey","Antarctica/Davis","Antarctica/DumontDUrville","Antarctica/Macquarie","Antarctica/Mawson","Antarctica/Palmer","Antarctica/Rothera","Antarctica/Syowa","Antarctica/Troll","Antarctica/Vostok","Asia/Almaty","Asia/Amman","Asia/Anadyr","Asia/Aqtau","Asia/Aqtobe","Asia/Ashgabat","Asia/Atyrau","Asia/Baghdad","Asia/Baku","Asia/Bangkok","Asia/Barnaul","Asia/Beirut","Asia/Bishkek","Asia/Brunei","Asia/Chita","Asia/Choibalsan","Asia/Colombo","Asia/Damascus","Asia/Dhaka","Asia/Dili","Asia/Dubai","Asia/Dushanbe","Asia/Famagusta","Asia/Gaza","Asia/Hebron","Asia/Ho_Chi_Minh","Asia/Hong_Kong","Asia/Hovd","Asia/Irkutsk","Asia/Jakarta","Asia/Jayapura","Asia/Jerusalem","Asia/Kabul","Asia/Kamchatka","Asia/Karachi","Asia/Kathmandu","Asia/Khandyga","Asia/Kolkata","Asia/Krasnoyarsk","Asia/Kuala_Lumpur","Asia/Kuching","Asia/Macau","Asia/Magadan","Asia/Makassar","Asia/Manila","Asia/Nicosia","Asia/Novokuznetsk","Asia/Novosibirsk","Asia/Omsk","Asia/Oral","Asia/Pontianak","Asia/Pyongyang","Asia/Qatar","Asia/Qostanay","Asia/Qyzylorda","Asia/Riyadh","Asia/Sakhalin","Asia/Samarkand","Asia/Seoul","Asia/Shanghai","Asia/Singapore","Asia/Srednekolymsk","Asia/Taipei","Asia/Tashkent","Asia/Tbilisi","Asia/Tehran","Asia/Thimphu","Asia/Tokyo","Asia/Tomsk","Asia/Ulaanbaatar","Asia/Urumqi","Asia/Ust-Nera","Asia/Vladivostok","Asia/Yakutsk","Asia/Yangon","Asia/Yekaterinburg","Asia/Yerevan","Atlantic/Azores","Atlantic/Bermuda","Atlantic/Canary","Atlantic/Cape_Verde","Atlantic/Faroe","Atlantic/Madeira","Atlantic/Reykjavik","Atlantic/South_Georgia","Atlantic/Stanley","Australia/Adelaide","Australia/Brisbane","Australia/Broken_Hill","Australia/Currie","Australia/Darwin","Australia/Eucla","Australia/Hobart","Australia/Lindeman","Australia/Lord_Howe","Australia/Melbourne","Australia/Perth","Australia/Sydney","Europe/Amsterdam","Europe/Andorra","Europe/Astrakhan","Europe/Athens","Europe/Belgrade","Europe/Berlin","Europe/Brussels","Europe/Bucharest","Europe/Budapest","Europe/Chisinau","Europe/Copenhagen","Europe/Dublin","Europe/Gibraltar","Europe/Helsinki","Europe/Istanbul","Europe/Kaliningrad","Europe/Kiev","Europe/Kirov","Europe/Lisbon","Europe/London","Europe/Luxembourg","Europe/Madrid","Europe/Malta","Europe/Minsk","Europe/Monaco","Europe/Moscow","Europe/Oslo","Europe/Paris","Europe/Prague","Europe/Riga","Europe/Rome","Europe/Samara","Europe/Saratov","Europe/Simferopol","Europe/Sofia","Europe/Stockholm","Europe/Tallinn","Europe/Tirane","Europe/Ulyanovsk","Europe/Uzhgorod","Europe/Vienna","Europe/Vilnius","Europe/Volgograd","Europe/Warsaw","Europe/Zaporozhye","Europe/Zurich","Indian/Chagos","Indian/Christmas","Indian/Cocos","Indian/Kerguelen","Indian/Mahe","Indian/Maldives","Indian/Mauritius","Indian/Reunion","Pacific/Apia","Pacific/Auckland","Pacific/Bougainville","Pacific/Chatham","Pacific/Chuuk","Pacific/Easter","Pacific/Efate","Pacific/Enderbury","Pacific/Fakaofo","Pacific/Fiji","Pacific/Funafuti","Pacific/Galapagos","Pacific/Gambier","Pacific/Guadalcanal","Pacific/Guam","Pacific/Honolulu","Pacific/Kiritimati","Pacific/Kosrae","Pacific/Kwajalein","Pacific/Majuro","Pacific/Marquesas","Pacific/Nauru","Pacific/Niue","Pacific/Norfolk","Pacific/Noumea","Pacific/Pago_Pago","Pacific/Palau","Pacific/Pitcairn","Pacific/Pohnpei","Pacific/Port_Moresby","Pacific/Rarotonga","Pacific/Tahiti","Pacific/Tarawa","Pacific/Tongatapu","Pacific/Wake","Pacific/Wallis"];!function(e,i){"object"==typeof exports&&"undefined"!=typeof module?module.exports=i():"function"==typeof define&&define.amd?define(i):e.tinybind=i()}(this,(function(){"use strict";var e=["prefix","templateDelimiters","rootInterface","preloadData","handler"],i=["binders","formatters","adapters"],t=/^'.*'$|^".*"$/;function a(e){var i=0,a=e;return t.test(e)?a=e.slice(1,-1):"true"===e?a=!0:"false"===e?a=!1:"null"===e?a=null:"undefined"===e?a=void 0:isNaN(e)?i=1:a=Number(e),{type:i,value:a}}function n(e,i){for(var t,a=e.length,n=0,r=0,o=i[0],s=i[1];r<a;){if((n=e.indexOf(o,r))<0){t&&t.push({type:0,value:e.slice(r)});break}if(t||(t=[]),0<n&&r<n&&t.push({type:0,value:e.slice(r,n)}),r=n+o.length,(n=e.indexOf(s,r))<0){var c=e.slice(r-s.length),u=t[t.length-1];u&&0===u.type?u.value+=c:t.push({type:0,value:c});break}var l=e.slice(r,n).trim();t.push({type:1,value:l}),r=n+s.length}return t}var r,o,s,c={binders:{},formatters:{},adapters:{},_prefix:"rv",_fullPrefix:"rv-",get prefix(){return this._prefix},set prefix(e){this._prefix=e,this._fullPrefix=e+"-"},parseTemplate:n,parseType:a,templateDelimiters:["{","}"],rootInterface:".",preloadData:!0,handler:function(e,i,t){this.call(e,i,t.view.models)},fallbackBinder:function(e,i){null!=i?e.setAttribute(this.type,i):e.removeAttribute(this.type)},configure:function(e){var t=this;e&&Object.keys(e).forEach((function(a){var n=e[a];-1<i.indexOf(a)?Object.keys(n).forEach((function(e){t[a][e]=n[e]})):t[a]=n}))}};function u(e){return"object"==typeof e&&null!==e}function l(e,i,t){this.keypath=i,this.callback=t,this.objectPath=[],this.parse(),this.obj=this.getRootObject(e),u(this.target=this.realize())&&this.set(!0,this.key,this.target,this.callback)}l.updateOptions=function(e){r=e.adapters,o=Object.keys(r),s=e.rootInterface},l.tokenize=function(e,i){var t,a,n=[],r={i,path:""};for(t=0;t<e.length;t++)a=e.charAt(t),~o.indexOf(a)?(n.push(r),r={i:a,path:""}):r.path+=a;return n.push(r),n},l.prototype.parse=function(){var e,i;o.length||function(e){throw new Error("[Observer] Must define at least one adapter interface.")}(),~o.indexOf(this.keypath[0])?(i=this.keypath[0],e=this.keypath.substr(1)):(i=s,e=this.keypath),this.tokens=l.tokenize(e,i),this.key=this.tokens.pop()},l.prototype.realize=function(){for(var e,i,t=this.obj,a=-1,n=0;n<this.tokens.length;n++)i=this.tokens[n],u(t)?(void 0!==this.objectPath[n]?t!==(e=this.objectPath[n])&&(this.set(!1,i,e,this),this.set(!0,i,t,this),this.objectPath[n]=t):(this.set(!0,i,t,this),this.objectPath[n]=t),t=this.get(i,t)):(-1===a&&(a=n),(e=this.objectPath[n])&&this.set(!1,i,e,this));return-1!==a&&this.objectPath.splice(a),t},l.prototype.sync=function(){var e,i,t;(e=this.realize())!==this.target?(u(this.target)&&this.set(!1,this.key,this.target,this.callback),u(e)&&this.set(!0,this.key,e,this.callback),i=this.value(),this.target=e,((t=this.value())!==i||t instanceof Function)&&this.callback.sync()):e instanceof Array&&this.callback.sync()},l.prototype.value=function(){if(u(this.target))return this.get(this.key,this.target)},l.prototype.setValue=function(e){u(this.target)&&r[this.key.i].set(this.target,this.key.path,e)},l.prototype.get=function(e,i){return r[e.i].get(i,e.path)},l.prototype.set=function(e,i,t,a){var n=e?"observe":"unobserve";r[i.i][n](t,i.path,a)},l.prototype.unobserve=function(){for(var e,i,t=0;t<this.tokens.length;t++)i=this.tokens[t],(e=this.objectPath[t])&&this.set(!1,i,e,this);u(this.target)&&this.set(!1,this.key,this.target,this.callback)},l.prototype.getRootObject=function(e){var i,t;if(!e.$parent)return e;for(i=this.tokens.length?this.tokens[0].path:this.key.path,t=e;t.$parent&&void 0===t[i];)t=t.$parent;return t};var h=function(e,i){if(!(e instanceof i))throw new TypeError("Cannot call a class as a function")},f=/[^\s']+|'([^']|'[^\s])*'|"([^"]|"[^\s])*"/g,d=/\s+/,p=function(){function e(i,t,a,n,r,o,s){h(this,e),this.view=i,this.el=t,this.type=a,this.keypath=n,this.binder=r,this.arg=o,this.formatters=s,this.formatterObservers={},this.model=void 0}return e.prototype.observe=function(e,i){return new l(e,i,this)},e.prototype.parseTarget=function(){if(this.keypath){var e=a(this.keypath);0===e.type?this.value=e.value:(this.observer=this.observe(this.view.models,this.keypath),this.model=this.observer.target)}else this.value=void 0},e.prototype.parseFormatterArguments=function(e,i){var t=this;return e.map(a).map((function(e,a){var n=e.type,r=e.value;if(0===n)return r;t.formatterObservers[i]||(t.formatterObservers[i]={});var o=t.formatterObservers[i][a];return o||(o=t.observe(t.view.models,r),t.formatterObservers[i][a]=o),o.value()}))},e.prototype.formattedValue=function(e){var i=this;return this.formatters.reduce((function(e,t,a){var n=t.match(f),r=n.shift(),o=i.view.options.formatters[r],s=i.parseFormatterArguments(n,a);return o&&o.read instanceof Function?e=o.read.apply(o,[e].concat(s)):o instanceof Function&&(e=o.apply(void 0,[e].concat(s))),e}),e)},e.prototype.eventHandler=function(e){var i=this,t=i.view.options.handler;return function(a){t.call(e,this,a,i)}},e.prototype.set=function(e){e=e instanceof Function&&!this.binder.function?this.formattedValue(e.call(this.model)):this.formattedValue(e);var i=this.binder.routine||this.binder;i instanceof Function&&i.call(this,this.el,e)},e.prototype.sync=function(){this.observer?(this.model=this.observer.target,this.set(this.observer.value())):this.set(this.value)},e.prototype.publish=function(){var e=this;if(this.observer){var i=this.formatters.reduceRight((function(i,t,a){var n=t.split(d),r=n.shift(),o=e.view.options.formatters[r],s=e.parseFormatterArguments(n,a);return o&&o.publish&&(i=o.publish.apply(o,[i].concat(s))),i}),this.getValue(this.el));this.observer.setValue(i)}},e.prototype.bind=function(){this.parseTarget(),this.binder.hasOwnProperty("bind")&&this.binder.bind.call(this,this.el),this.view.options.preloadData&&this.sync()},e.prototype.unbind=function(){var e=this;this.binder.unbind&&this.binder.unbind.call(this,this.el),this.observer&&this.observer.unobserve(),Object.keys(this.formatterObservers).forEach((function(i){var t=e.formatterObservers[i];Object.keys(t).forEach((function(e){t[e].unobserve()}))})),this.formatterObservers={}},e.prototype.update=function(){var e=0<arguments.length&&void 0!==arguments[0]?arguments[0]:{};this.observer&&(this.model=this.observer.target),this.binder.update&&this.binder.update.call(this,e)},e.prototype.getValue=function(e){return this.binder&&this.binder.getValue?this.binder.getValue.call(this,e):(t=[],"checkbox"===(i=e).type?i.checked:"select-multiple"===i.type?(i.options.forEach((function(e){e.selected&&t.push(e.value)})),t):i.value);var i,t},e}(),m={routine:function(e,i){e.data=null!=i?i:""}},A=/((?:'[^']*')*(?:(?:[^\|']*(?:'[^']*')+[^\|']*)+|[^\|]+))|^$/g,b=function e(i,t){var a=!1;if(3===t.nodeType){var r=n(t.data,c.templateDelimiters);if(r){for(var o=0;o<r.length;o++){var s=r[o],u=document.createTextNode(s.value);t.parentNode.insertBefore(u,t),1===s.type&&i.buildBinding(u,null,s.value,m,null)}t.parentNode.removeChild(t)}a=!0}else 1===t.nodeType&&(a=i.traverse(t));if(!a)for(var l=0;l<t.childNodes.length;l++)e(i,t.childNodes[l])},v=function(e,i){var t=e.binder&&e.binder.priority||0;return(i.binder&&i.binder.priority||0)-t},g=function(e){return e.trim()},y=function(){function e(i,t,a){h(this,e),i.jquery||i instanceof Array?this.els=i:this.els=[i],this.models=t,this.options=a,this.build()}return e.prototype.buildBinding=function(e,i,t,a,n){var r=t.match(A).map(g),o=r.shift();this.bindings.push(new p(this,e,i,o,a,n,r))},e.prototype.build=function(){this.bindings=[];var e,i=this.els,t=void 0;for(t=0,e=i.length;t<e;t++)b(this,i[t]);this.bindings.sort(v)},e.prototype.traverse=function(e){for(var i,t,a,n,r=c._fullPrefix,o="SCRIPT"===e.nodeName||"STYLE"===e.nodeName,s=e.attributes,u=[],l=this.options.starBinders,h=0,f=s.length;h<f;h++){var d=s[h];if(0===d.name.indexOf(r)){if(i=d.name.slice(r.length),n=void 0,!(t=this.options.binders[i]))for(var p=0;p<l.length;p++)if(a=l[p],i.slice(0,a.length-1)===a.slice(0,-1)){t=this.options.binders[a],n=i.slice(a.length-1);break}if(t||(t=c.fallbackBinder),t.block)return this.buildBinding(e,i,d.value,t,n),e.removeAttribute(d.name),!0;u.push({attr:d,binder:t,type:i,arg:n})}}for(var m=0;m<u.length;m++){var A=u[m];this.buildBinding(e,A.type,A.attr.value,A.binder,A.arg),e.removeAttribute(A.attr.name)}return o},e.prototype.bind=function(){this.bindings.forEach((function(e){e.bind()}))},e.prototype.unbind=function(){this.bindings.forEach((function(e){e.unbind()}))},e.prototype.sync=function(){this.bindings.forEach((function(e){e.sync()}))},e.prototype.publish=function(){this.bindings.forEach((function(e){e.binder&&e.binder.publishes&&e.publish()}))},e.prototype.update=function(){var e=this,i=0<arguments.length&&void 0!==arguments[0]?arguments[0]:{};Object.keys(i).forEach((function(t){e.models[t]=i[t]})),this.bindings.forEach((function(e){e.update&&e.update(i)}))},e}(),k=["push","pop","shift","unshift","sort","reverse","splice"],E={counter:0,weakmap:{},weakReference:function(e){if(!e.hasOwnProperty("__rv")){var i=this.counter++;Object.defineProperty(e,"__rv",{value:i})}return this.weakmap[e.__rv]||(this.weakmap[e.__rv]={callbacks:{}}),this.weakmap[e.__rv]},cleanupWeakReference:function(e,i){Object.keys(e.callbacks).length||e.pointers&&Object.keys(e.pointers).length||delete this.weakmap[i]},stubFunction:function(e,i){var t=e[i],a=this.weakReference(e),n=this.weakmap;e[i]=function(){for(var i=arguments.length,r=Array(i),o=0;o<i;o++)r[o]=arguments[o];var s=t.apply(e,r);return Object.keys(a.pointers).forEach((function(e){var i=a.pointers[e];n[e]&&n[e].callbacks[i]instanceof Array&&n[e].callbacks[i].forEach((function(e){e.sync()}))})),s}},observeMutations:function(e,i,t){var a=this;if(e instanceof Array){var n=this.weakReference(e);n.pointers||(n.pointers={},k.forEach((function(i){a.stubFunction(e,i)}))),n.pointers[i]||(n.pointers[i]=[]),-1===n.pointers[i].indexOf(t)&&n.pointers[i].push(t)}},unobserveMutations:function(e,i,t){if(e instanceof Array&&null!=e.__rv){var a=this.weakmap[e.__rv];if(a){var n=a.pointers[i];if(n){var r=n.indexOf(t);-1<r&&n.splice(r,1),n.length||delete a.pointers[i],this.cleanupWeakReference(a,e.__rv)}}}},observe:function(e,i,t){var a,n=this,r=this.weakReference(e).callbacks;if(!r[i]){r[i]=[];var o=Object.getOwnPropertyDescriptor(e,i);o&&(o.get||o.set||!o.configurable)||(a=e[i],Object.defineProperty(e,i,{enumerable:!0,get:function(){return a},set:function(t){if(t!==a){n.unobserveMutations(a,e.__rv,i),a=t;var r=n.weakmap[e.__rv];if(r){var o=r.callbacks[i];o&&o.forEach((function(e){e.sync()})),n.observeMutations(t,e.__rv,i)}}}}))}-1===r[i].indexOf(t)&&r[i].push(t),this.observeMutations(e[i],e.__rv,i)},unobserve:function(e,i,t){var a=this.weakmap[e.__rv];if(a){var n=a.callbacks[i];if(n){var r=n.indexOf(t);-1<r&&(n.splice(r,1),n.length||(delete a.callbacks[i],this.unobserveMutations(e[i],e.__rv,i))),this.cleanupWeakReference(a,e.__rv)}}},get:function(e,i){return e[i]},set:function(e,i,t){e[i]=t}},_=function(e){return null!=e?e.toString():void 0};function P(e,i,t){var a=e.el.cloneNode(!0),n=new y(a,i,e.view.options);return n.bind(),e.marker.parentNode.insertBefore(a,t),n}var w={"on-*":{function:!0,priority:1e3,unbind:function(e){this.handler&&e.removeEventListener(this.arg,this.handler)},routine:function(e,i){this.handler&&e.removeEventListener(this.arg,this.handler),this.handler=this.eventHandler(i),e.addEventListener(this.arg,this.handler)}},"each-*":{block:!0,priority:4e3,bind:function(e){this.marker?this.iterated.forEach((function(e){e.bind()})):(this.marker=document.createComment(" tinybind: "+this.type+" "),this.iterated=[],e.parentNode.insertBefore(this.marker,e),e.parentNode.removeChild(e))},unbind:function(e){this.iterated&&this.iterated.forEach((function(e){e.unbind()}))},routine:function(e,i){var t=this,a=this.arg;i=i||[];var n=e.getAttribute("index-property")||"$index";i.forEach((function(e,i){var r={$parent:t.view.models};r[n]=i,r[a]=e;var o=t.iterated[i];if(o)if(o.models[a]!==e){for(var s=void 0,c=void 0,u=i+1;u<t.iterated.length;u++)if((c=t.iterated[u]).models[a]===e){s=u;break}void 0!==s?(t.iterated.splice(s,1),t.marker.parentNode.insertBefore(c.els[0],o.els[0]),c.models[n]=i):c=P(t,r,o.els[0]),t.iterated.splice(i,0,c)}else o.models[n]=i;else{var l=t.marker;t.iterated.length&&(l=t.iterated[t.iterated.length-1].els[0]),o=P(t,r,l.nextSibling),t.iterated.push(o)}})),this.iterated.length>i.length&&function(e,i){for(var t=0;t<e;t++)i()}(this.iterated.length-i.length,(function(){var e=t.iterated.pop();e.unbind(),t.marker.parentNode.removeChild(e.els[0])})),"OPTION"===e.nodeName&&this.view.bindings.forEach((function(e){e.el===t.marker.parentNode&&"value"===e.type&&e.sync()}))},update:function(e){var i=this,t={};Object.keys(e).forEach((function(a){a!==i.arg&&(t[a]=e[a])})),this.iterated.forEach((function(e){e.update(t)}))}},"class-*":function(e,i){var t=" "+e.className+" ";!i==-1<t.indexOf(" "+this.arg+" ")&&(e.className=i?e.className+" "+this.arg:t.replace(" "+this.arg+" "," ").trim())},text:function(e,i){e.textContent=null!=i?i:""},html:function(e,i){e.innerHTML=null!=i?i:""},show:function(e,i){e.style.display=i?"":"none"},hide:function(e,i){e.style.display=i?"none":""},enabled:function(e,i){e.disabled=!i},disabled:function(e,i){e.disabled=!!i},checked:{publishes:!0,priority:2e3,bind:function(e){var i=this;this.callback||(this.callback=function(){i.publish()}),e.addEventListener("change",this.callback)},unbind:function(e){e.removeEventListener("change",this.callback)},routine:function(e,i){"radio"===e.type?e.checked=_(e.value)===_(i):e.checked=!!i}},value:{publishes:!0,priority:3e3,bind:function(e){if(this.isRadio="INPUT"===e.tagName&&"radio"===e.type,!this.isRadio){this.event=e.getAttribute("event-name")||("SELECT"===e.tagName?"change":"input");var i=this;this.callback||(this.callback=function(){i.publish()}),e.addEventListener(this.event,this.callback)}},unbind:function(e){this.isRadio||e.removeEventListener(this.event,this.callback)},routine:function(e,i){if(this.isRadio)e.setAttribute("value",i);else if("select-multiple"===e.type){if(i instanceof Array)for(var t=0;t<e.length;t++){var a=e[t];a.selected=-1<i.indexOf(a.value)}}else _(i)!==_(e.value)&&(e.value=null!=i?i:"")}},if:{block:!0,priority:4e3,bind:function(e){this.marker?!1===this.bound&&this.nested&&this.nested.bind():(this.marker=document.createComment(" tinybind: "+this.type+" "+this.keypath+" "),this.attached=!1,e.parentNode.insertBefore(this.marker,e),e.parentNode.removeChild(e)),this.bound=!0},unbind:function(){this.nested&&(this.nested.unbind(),this.bound=!1)},routine:function(e,i){!!i!==this.attached&&(i?(this.nested||(this.nested=new y(e,this.view.models,this.view.options),this.nested.bind()),this.marker.parentNode.insertBefore(e,this.marker.nextSibling),this.attached=!0):(e.parentNode.removeChild(e),this.attached=!1))},update:function(e){this.nested&&this.nested.update(e)}}};return c.binders=w,c.adapters["."]=E,c.bind=function(t,a,n){var r={};a=a||{},n=n||{},i.forEach((function(e){r[e]=Object.create(null),n[e]&&Object.keys(n[e]).forEach((function(i){r[e][i]=n[e][i]})),Object.keys(c[e]).forEach((function(i){r[e][i]||(r[e][i]=c[e][i])}))})),e.forEach((function(e){var i=n[e];r[e]=null!=i?i:c[e]})),r.starBinders=Object.keys(r.binders).filter((function(e){return 0<e.indexOf("*")})),l.updateOptions(r);var o=new y(t,a,r);return o.bind(),o},c.formatters.negate=c.formatters.not=function(e){return!e},c}));let app,config={},time=0,version=0;document.addEventListener("DOMContentLoaded",(function(e){initTabs(),tinybind.formatters.sub=function(e,i){return e-i},tinybind.binders.barwidth=function(e,i){let t=i/config.ledCount*100;e.style.width=t+"%"},tinybind.binders.barpos=function(e,i){let t=(i-1)/config.ledCount*100;e.style.marginLeft=t+"%"},tinybind.formatters.addEmoji=function(e,i){return i+e},tinybind.formatters.replaceUnderscore=function(e){return e.replace(/_/g," ")},tinybind.formatters.formatDate=function(e){if(0!=e){return new Date(1e3*e).toLocaleString()}return""},tinybind.binders.baroverflow=function(e){e.scrollHeight>e.clientHeight||e.scrollWidth>e.clientWidth?console.log("overflow"):console.log("no overflow")},getTime().then((function(e){time=e})),getVersion().then((function(e){version=e})),loadConfig().then((function(e){config=e})).then((function(){app=tinybind.bind(document.getElementById("app"),{config,version,time,timezones,languages,toggleFirmwareModal,toggleResetModal,loadLanguage})})),window.setInterval((function(){getTime().then((function(e){app.models.time=e}))}),1e3)}));