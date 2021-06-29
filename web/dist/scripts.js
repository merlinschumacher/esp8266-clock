async function postConfig(e=!1){return!!document.getElementById("configform").checkValidity()&&(conf=app.models.config,conf.saveData=e,e&&document.getElementById("save-button").classList.add("loading"),await fetch("data.json",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify(conf)}).then((function(e){return e.json()})).then((t=>{e&&(document.getElementById("save-button").classList.remove("loading"),document.getElementById("save-button").classList.remove("badge"),showToast("toast-success",2,!0))})).catch((e=>{throw console.error("Error:",e),showToast("toast-error",1),document.getElementById("save-button").classList.remove("loading"),new Error("Failed to save settings!")})))}async function getConfig(){return await fetch("/data.json").then((function(e){if(e.ok)return e.json();throw new Error("Failed to load settings!")})).then((function(e){return e}))}async function getData(e){return await fetch(e).then((function(t){if(t.ok)return t.text();throw new Error("Failed to load "+e+" !")})).then((function(e){return e}))}const languages=[{code:"en",name:"English",icon:"🇺🇸"},{code:"de",name:"Deutsch",icon:"🇩🇪"}],loadLanguage=function(){setTimeout((function(){postConfig(!0).then((function(){location.reload()}))}),250)},toggleResetModal=function(){return document.getElementById("modalreset").classList.toggle("active"),!1},toggleFirmwareModal=function(){return document.getElementById("modalfirmware").classList.toggle("active"),!1};function switchTab(e){const t=e.getAttribute("href").substring(1),i=document.querySelectorAll(".contenttab");document.querySelectorAll("li.tab-item").forEach((e=>{e.id.includes(t)?e.classList.add("active"):e.classList.remove("active")})),i.forEach((e=>{e.id==t?e.style.display="unset":e.style.display="none"}))}function initTabs(){document.getElementById("colorconfig").style.display="unset";document.querySelectorAll(".tab-item > a").forEach((e=>{e.addEventListener("click",(function(e){return switchTab(e.target),!1}),!1)}))}const timezones=["Africa/Abidjan","Africa/Accra","Africa/Algiers","Africa/Bissau","Africa/Cairo","Africa/Casablanca","Africa/Ceuta","Africa/El_Aaiun","Africa/Johannesburg","Africa/Juba","Africa/Khartoum","Africa/Lagos","Africa/Maputo","Africa/Monrovia","Africa/Nairobi","Africa/Ndjamena","Africa/Sao_Tome","Africa/Tripoli","Africa/Tunis","Africa/Windhoek","America/Adak","America/Anchorage","America/Araguaina","America/Argentina/Buenos_Aires","America/Argentina/Catamarca","America/Argentina/Cordoba","America/Argentina/Jujuy","America/Argentina/La_Rioja","America/Argentina/Mendoza","America/Argentina/Rio_Gallegos","America/Argentina/Salta","America/Argentina/San_Juan","America/Argentina/San_Luis","America/Argentina/Tucuman","America/Argentina/Ushuaia","America/Asuncion","America/Atikokan","America/Bahia","America/Bahia_Banderas","America/Barbados","America/Belem","America/Belize","America/Blanc-Sablon","America/Boa_Vista","America/Bogota","America/Boise","America/Cambridge_Bay","America/Campo_Grande","America/Cancun","America/Caracas","America/Cayenne","America/Chicago","America/Chihuahua","America/Costa_Rica","America/Creston","America/Cuiaba","America/Curacao","America/Danmarkshavn","America/Dawson","America/Dawson_Creek","America/Denver","America/Detroit","America/Edmonton","America/Eirunepe","America/El_Salvador","America/Fort_Nelson","America/Fortaleza","America/Glace_Bay","America/Goose_Bay","America/Grand_Turk","America/Guatemala","America/Guayaquil","America/Guyana","America/Halifax","America/Havana","America/Hermosillo","America/Indiana/Indianapolis","America/Indiana/Knox","America/Indiana/Marengo","America/Indiana/Petersburg","America/Indiana/Tell_City","America/Indiana/Vevay","America/Indiana/Vincennes","America/Indiana/Winamac","America/Inuvik","America/Iqaluit","America/Jamaica","America/Juneau","America/Kentucky/Louisville","America/Kentucky/Monticello","America/La_Paz","America/Lima","America/Los_Angeles","America/Maceio","America/Managua","America/Manaus","America/Martinique","America/Matamoros","America/Mazatlan","America/Menominee","America/Merida","America/Metlakatla","America/Mexico_City","America/Miquelon","America/Moncton","America/Monterrey","America/Montevideo","America/Nassau","America/New_York","America/Nipigon","America/Nome","America/Noronha","America/North_Dakota/Beulah","America/North_Dakota/Center","America/North_Dakota/New_Salem","America/Nuuk","America/Ojinaga","America/Panama","America/Pangnirtung","America/Paramaribo","America/Phoenix","America/Port_of_Spain","America/Port-au-Prince","America/Porto_Velho","America/Puerto_Rico","America/Punta_Arenas","America/Rainy_River","America/Rankin_Inlet","America/Recife","America/Regina","America/Resolute","America/Rio_Branco","America/Santarem","America/Santiago","America/Santo_Domingo","America/Sao_Paulo","America/Scoresbysund","America/Sitka","America/St_Johns","America/Swift_Current","America/Tegucigalpa","America/Thule","America/Thunder_Bay","America/Tijuana","America/Toronto","America/Vancouver","America/Whitehorse","America/Winnipeg","America/Yakutat","America/Yellowknife","Antarctica/Casey","Antarctica/Davis","Antarctica/DumontDUrville","Antarctica/Macquarie","Antarctica/Mawson","Antarctica/Palmer","Antarctica/Rothera","Antarctica/Syowa","Antarctica/Troll","Antarctica/Vostok","Asia/Almaty","Asia/Amman","Asia/Anadyr","Asia/Aqtau","Asia/Aqtobe","Asia/Ashgabat","Asia/Atyrau","Asia/Baghdad","Asia/Baku","Asia/Bangkok","Asia/Barnaul","Asia/Beirut","Asia/Bishkek","Asia/Brunei","Asia/Chita","Asia/Choibalsan","Asia/Colombo","Asia/Damascus","Asia/Dhaka","Asia/Dili","Asia/Dubai","Asia/Dushanbe","Asia/Famagusta","Asia/Gaza","Asia/Hebron","Asia/Ho_Chi_Minh","Asia/Hong_Kong","Asia/Hovd","Asia/Irkutsk","Asia/Jakarta","Asia/Jayapura","Asia/Jerusalem","Asia/Kabul","Asia/Kamchatka","Asia/Karachi","Asia/Kathmandu","Asia/Khandyga","Asia/Kolkata","Asia/Krasnoyarsk","Asia/Kuala_Lumpur","Asia/Kuching","Asia/Macau","Asia/Magadan","Asia/Makassar","Asia/Manila","Asia/Nicosia","Asia/Novokuznetsk","Asia/Novosibirsk","Asia/Omsk","Asia/Oral","Asia/Pontianak","Asia/Pyongyang","Asia/Qatar","Asia/Qostanay","Asia/Qyzylorda","Asia/Riyadh","Asia/Sakhalin","Asia/Samarkand","Asia/Seoul","Asia/Shanghai","Asia/Singapore","Asia/Srednekolymsk","Asia/Taipei","Asia/Tashkent","Asia/Tbilisi","Asia/Tehran","Asia/Thimphu","Asia/Tokyo","Asia/Tomsk","Asia/Ulaanbaatar","Asia/Urumqi","Asia/Ust-Nera","Asia/Vladivostok","Asia/Yakutsk","Asia/Yangon","Asia/Yekaterinburg","Asia/Yerevan","Atlantic/Azores","Atlantic/Bermuda","Atlantic/Canary","Atlantic/Cape_Verde","Atlantic/Faroe","Atlantic/Madeira","Atlantic/Reykjavik","Atlantic/South_Georgia","Atlantic/Stanley","Australia/Adelaide","Australia/Brisbane","Australia/Broken_Hill","Australia/Currie","Australia/Darwin","Australia/Eucla","Australia/Hobart","Australia/Lindeman","Australia/Lord_Howe","Australia/Melbourne","Australia/Perth","Australia/Sydney","Europe/Amsterdam","Europe/Andorra","Europe/Astrakhan","Europe/Athens","Europe/Belgrade","Europe/Berlin","Europe/Brussels","Europe/Bucharest","Europe/Budapest","Europe/Chisinau","Europe/Copenhagen","Europe/Dublin","Europe/Gibraltar","Europe/Helsinki","Europe/Istanbul","Europe/Kaliningrad","Europe/Kiev","Europe/Kirov","Europe/Lisbon","Europe/London","Europe/Luxembourg","Europe/Madrid","Europe/Malta","Europe/Minsk","Europe/Monaco","Europe/Moscow","Europe/Oslo","Europe/Paris","Europe/Prague","Europe/Riga","Europe/Rome","Europe/Samara","Europe/Saratov","Europe/Simferopol","Europe/Sofia","Europe/Stockholm","Europe/Tallinn","Europe/Tirane","Europe/Ulyanovsk","Europe/Uzhgorod","Europe/Vienna","Europe/Vilnius","Europe/Volgograd","Europe/Warsaw","Europe/Zaporozhye","Europe/Zurich","Indian/Chagos","Indian/Christmas","Indian/Cocos","Indian/Kerguelen","Indian/Mahe","Indian/Maldives","Indian/Mauritius","Indian/Reunion","Pacific/Apia","Pacific/Auckland","Pacific/Bougainville","Pacific/Chatham","Pacific/Chuuk","Pacific/Easter","Pacific/Efate","Pacific/Enderbury","Pacific/Fakaofo","Pacific/Fiji","Pacific/Funafuti","Pacific/Galapagos","Pacific/Gambier","Pacific/Guadalcanal","Pacific/Guam","Pacific/Honolulu","Pacific/Kiritimati","Pacific/Kosrae","Pacific/Kwajalein","Pacific/Majuro","Pacific/Marquesas","Pacific/Nauru","Pacific/Niue","Pacific/Norfolk","Pacific/Noumea","Pacific/Pago_Pago","Pacific/Palau","Pacific/Pitcairn","Pacific/Pohnpei","Pacific/Port_Moresby","Pacific/Rarotonga","Pacific/Tahiti","Pacific/Tarawa","Pacific/Tongatapu","Pacific/Wake","Pacific/Wallis"];function showToast(e,t,i=!1){app.models.toastVisible=!0,toast=document.getElementById("header-toast"),document.getElementById("header-toast").style.opacity="1",document.getElementById("header-toast").style.display="";const a=toast.className.split(" ").filter((e=>!e.startsWith("toast-")));toast.className=a.join(" ").trim(),spans=toast.children;for(let i=0;i<spans.length;i++)i==t?(spans[i].classList.remove("d-none"),toast.classList.add(e)):spans[i].classList.add("d-none");i&&(setTimeout((function(){document.getElementById("header-toast").style.opacity="0"}),3e3),setTimeout((function(){document.getElementById("header-toast").style.display="none"}),4e3))}function initWatcher(){document.querySelectorAll("#configform input, #configform select").forEach((e=>{e.addEventListener("change",(function(){document.getElementById("save-button").classList.add("badge"),postConfig(!1)}))}))}!function(e,t){"object"==typeof exports&&"undefined"!=typeof module?module.exports=t():"function"==typeof define&&define.amd?define(t):e.tinybind=t()}(this,(function(){"use strict";var e=["prefix","templateDelimiters","rootInterface","preloadData","handler"],t=["binders","formatters","adapters"],i=/^'.*'$|^".*"$/;function a(e){var t=0,a=e;return i.test(e)?a=e.slice(1,-1):"true"===e?a=!0:"false"===e?a=!1:"null"===e?a=null:"undefined"===e?a=void 0:isNaN(e)?t=1:a=Number(e),{type:t,value:a}}function n(e,t){for(var i,a=e.length,n=0,r=0,o=t[0],s=t[1];r<a;){if((n=e.indexOf(o,r))<0){i&&i.push({type:0,value:e.slice(r)});break}if(i||(i=[]),0<n&&r<n&&i.push({type:0,value:e.slice(r,n)}),r=n+o.length,(n=e.indexOf(s,r))<0){var c=e.slice(r-s.length),u=i[i.length-1];u&&0===u.type?u.value+=c:i.push({type:0,value:c});break}var l=e.slice(r,n).trim();i.push({type:1,value:l}),r=n+s.length}return i}var r,o,s,c={binders:{},formatters:{},adapters:{},_prefix:"rv",_fullPrefix:"rv-",get prefix(){return this._prefix},set prefix(e){this._prefix=e,this._fullPrefix=e+"-"},parseTemplate:n,parseType:a,templateDelimiters:["{","}"],rootInterface:".",preloadData:!0,handler:function(e,t,i){this.call(e,t,i.view.models)},fallbackBinder:function(e,t){null!=t?e.setAttribute(this.type,t):e.removeAttribute(this.type)},configure:function(e){var i=this;e&&Object.keys(e).forEach((function(a){var n=e[a];-1<t.indexOf(a)?Object.keys(n).forEach((function(e){i[a][e]=n[e]})):i[a]=n}))}};function u(e){return"object"==typeof e&&null!==e}function l(e,t,i){this.keypath=t,this.callback=i,this.objectPath=[],this.parse(),this.obj=this.getRootObject(e),u(this.target=this.realize())&&this.set(!0,this.key,this.target,this.callback)}l.updateOptions=function(e){r=e.adapters,o=Object.keys(r),s=e.rootInterface},l.tokenize=function(e,t){var i,a,n=[],r={i:t,path:""};for(i=0;i<e.length;i++)a=e.charAt(i),~o.indexOf(a)?(n.push(r),r={i:a,path:""}):r.path+=a;return n.push(r),n},l.prototype.parse=function(){var e,t;o.length||function(e){throw new Error("[Observer] Must define at least one adapter interface.")}(),~o.indexOf(this.keypath[0])?(t=this.keypath[0],e=this.keypath.substr(1)):(t=s,e=this.keypath),this.tokens=l.tokenize(e,t),this.key=this.tokens.pop()},l.prototype.realize=function(){for(var e,t,i=this.obj,a=-1,n=0;n<this.tokens.length;n++)t=this.tokens[n],u(i)?(void 0!==this.objectPath[n]?i!==(e=this.objectPath[n])&&(this.set(!1,t,e,this),this.set(!0,t,i,this),this.objectPath[n]=i):(this.set(!0,t,i,this),this.objectPath[n]=i),i=this.get(t,i)):(-1===a&&(a=n),(e=this.objectPath[n])&&this.set(!1,t,e,this));return-1!==a&&this.objectPath.splice(a),i},l.prototype.sync=function(){var e,t,i;(e=this.realize())!==this.target?(u(this.target)&&this.set(!1,this.key,this.target,this.callback),u(e)&&this.set(!0,this.key,e,this.callback),t=this.value(),this.target=e,((i=this.value())!==t||i instanceof Function)&&this.callback.sync()):e instanceof Array&&this.callback.sync()},l.prototype.value=function(){if(u(this.target))return this.get(this.key,this.target)},l.prototype.setValue=function(e){u(this.target)&&r[this.key.i].set(this.target,this.key.path,e)},l.prototype.get=function(e,t){return r[e.i].get(t,e.path)},l.prototype.set=function(e,t,i,a){var n=e?"observe":"unobserve";r[t.i][n](i,t.path,a)},l.prototype.unobserve=function(){for(var e,t,i=0;i<this.tokens.length;i++)t=this.tokens[i],(e=this.objectPath[i])&&this.set(!1,t,e,this);u(this.target)&&this.set(!1,this.key,this.target,this.callback)},l.prototype.getRootObject=function(e){var t,i;if(!e.$parent)return e;for(t=this.tokens.length?this.tokens[0].path:this.key.path,i=e;i.$parent&&void 0===i[t];)i=i.$parent;return i};var h=function(e,t){if(!(e instanceof t))throw new TypeError("Cannot call a class as a function")},d=/[^\s']+|'([^']|'[^\s])*'|"([^"]|"[^\s])*"/g,f=/\s+/,m=function(){function e(t,i,a,n,r,o,s){h(this,e),this.view=t,this.el=i,this.type=a,this.keypath=n,this.binder=r,this.arg=o,this.formatters=s,this.formatterObservers={},this.model=void 0}return e.prototype.observe=function(e,t){return new l(e,t,this)},e.prototype.parseTarget=function(){if(this.keypath){var e=a(this.keypath);0===e.type?this.value=e.value:(this.observer=this.observe(this.view.models,this.keypath),this.model=this.observer.target)}else this.value=void 0},e.prototype.parseFormatterArguments=function(e,t){var i=this;return e.map(a).map((function(e,a){var n=e.type,r=e.value;if(0===n)return r;i.formatterObservers[t]||(i.formatterObservers[t]={});var o=i.formatterObservers[t][a];return o||(o=i.observe(i.view.models,r),i.formatterObservers[t][a]=o),o.value()}))},e.prototype.formattedValue=function(e){var t=this;return this.formatters.reduce((function(e,i,a){var n=i.match(d),r=n.shift(),o=t.view.options.formatters[r],s=t.parseFormatterArguments(n,a);return o&&o.read instanceof Function?e=o.read.apply(o,[e].concat(s)):o instanceof Function&&(e=o.apply(void 0,[e].concat(s))),e}),e)},e.prototype.eventHandler=function(e){var t=this,i=t.view.options.handler;return function(a){i.call(e,this,a,t)}},e.prototype.set=function(e){e=e instanceof Function&&!this.binder.function?this.formattedValue(e.call(this.model)):this.formattedValue(e);var t=this.binder.routine||this.binder;t instanceof Function&&t.call(this,this.el,e)},e.prototype.sync=function(){this.observer?(this.model=this.observer.target,this.set(this.observer.value())):this.set(this.value)},e.prototype.publish=function(){var e=this;if(this.observer){var t=this.formatters.reduceRight((function(t,i,a){var n=i.split(f),r=n.shift(),o=e.view.options.formatters[r],s=e.parseFormatterArguments(n,a);return o&&o.publish&&(t=o.publish.apply(o,[t].concat(s))),t}),this.getValue(this.el));this.observer.setValue(t)}},e.prototype.bind=function(){this.parseTarget(),this.binder.hasOwnProperty("bind")&&this.binder.bind.call(this,this.el),this.view.options.preloadData&&this.sync()},e.prototype.unbind=function(){var e=this;this.binder.unbind&&this.binder.unbind.call(this,this.el),this.observer&&this.observer.unobserve(),Object.keys(this.formatterObservers).forEach((function(t){var i=e.formatterObservers[t];Object.keys(i).forEach((function(e){i[e].unobserve()}))})),this.formatterObservers={}},e.prototype.update=function(){var e=0<arguments.length&&void 0!==arguments[0]?arguments[0]:{};this.observer&&(this.model=this.observer.target),this.binder.update&&this.binder.update.call(this,e)},e.prototype.getValue=function(e){return this.binder&&this.binder.getValue?this.binder.getValue.call(this,e):(i=[],"checkbox"===(t=e).type?t.checked:"select-multiple"===t.type?(t.options.forEach((function(e){e.selected&&i.push(e.value)})),i):t.value);var t,i},e}(),p={routine:function(e,t){e.data=null!=t?t:""}},A=/((?:'[^']*')*(?:(?:[^\|']*(?:'[^']*')+[^\|']*)+|[^\|]+))|^$/g,b=function e(t,i){var a=!1;if(3===i.nodeType){var r=n(i.data,c.templateDelimiters);if(r){for(var o=0;o<r.length;o++){var s=r[o],u=document.createTextNode(s.value);i.parentNode.insertBefore(u,i),1===s.type&&t.buildBinding(u,null,s.value,p,null)}i.parentNode.removeChild(i)}a=!0}else 1===i.nodeType&&(a=t.traverse(i));if(!a)for(var l=0;l<i.childNodes.length;l++)e(t,i.childNodes[l])},v=function(e,t){var i=e.binder&&e.binder.priority||0;return(t.binder&&t.binder.priority||0)-i},g=function(e){return e.trim()},y=function(){function e(t,i,a){h(this,e),t.jquery||t instanceof Array?this.els=t:this.els=[t],this.models=i,this.options=a,this.build()}return e.prototype.buildBinding=function(e,t,i,a,n){var r=i.match(A).map(g),o=r.shift();this.bindings.push(new m(this,e,t,o,a,n,r))},e.prototype.build=function(){this.bindings=[];var e,t=this.els,i=void 0;for(i=0,e=t.length;i<e;i++)b(this,t[i]);this.bindings.sort(v)},e.prototype.traverse=function(e){for(var t,i,a,n,r=c._fullPrefix,o="SCRIPT"===e.nodeName||"STYLE"===e.nodeName,s=e.attributes,u=[],l=this.options.starBinders,h=0,d=s.length;h<d;h++){var f=s[h];if(0===f.name.indexOf(r)){if(t=f.name.slice(r.length),n=void 0,!(i=this.options.binders[t]))for(var m=0;m<l.length;m++)if(a=l[m],t.slice(0,a.length-1)===a.slice(0,-1)){i=this.options.binders[a],n=t.slice(a.length-1);break}if(i||(i=c.fallbackBinder),i.block)return this.buildBinding(e,t,f.value,i,n),e.removeAttribute(f.name),!0;u.push({attr:f,binder:i,type:t,arg:n})}}for(var p=0;p<u.length;p++){var A=u[p];this.buildBinding(e,A.type,A.attr.value,A.binder,A.arg),e.removeAttribute(A.attr.name)}return o},e.prototype.bind=function(){this.bindings.forEach((function(e){e.bind()}))},e.prototype.unbind=function(){this.bindings.forEach((function(e){e.unbind()}))},e.prototype.sync=function(){this.bindings.forEach((function(e){e.sync()}))},e.prototype.publish=function(){this.bindings.forEach((function(e){e.binder&&e.binder.publishes&&e.publish()}))},e.prototype.update=function(){var e=this,t=0<arguments.length&&void 0!==arguments[0]?arguments[0]:{};Object.keys(t).forEach((function(i){e.models[i]=t[i]})),this.bindings.forEach((function(e){e.update&&e.update(t)}))},e}(),k=["push","pop","shift","unshift","sort","reverse","splice"],E={counter:0,weakmap:{},weakReference:function(e){if(!e.hasOwnProperty("__rv")){var t=this.counter++;Object.defineProperty(e,"__rv",{value:t})}return this.weakmap[e.__rv]||(this.weakmap[e.__rv]={callbacks:{}}),this.weakmap[e.__rv]},cleanupWeakReference:function(e,t){Object.keys(e.callbacks).length||e.pointers&&Object.keys(e.pointers).length||delete this.weakmap[t]},stubFunction:function(e,t){var i=e[t],a=this.weakReference(e),n=this.weakmap;e[t]=function(){for(var t=arguments.length,r=Array(t),o=0;o<t;o++)r[o]=arguments[o];var s=i.apply(e,r);return Object.keys(a.pointers).forEach((function(e){var t=a.pointers[e];n[e]&&n[e].callbacks[t]instanceof Array&&n[e].callbacks[t].forEach((function(e){e.sync()}))})),s}},observeMutations:function(e,t,i){var a=this;if(e instanceof Array){var n=this.weakReference(e);n.pointers||(n.pointers={},k.forEach((function(t){a.stubFunction(e,t)}))),n.pointers[t]||(n.pointers[t]=[]),-1===n.pointers[t].indexOf(i)&&n.pointers[t].push(i)}},unobserveMutations:function(e,t,i){if(e instanceof Array&&null!=e.__rv){var a=this.weakmap[e.__rv];if(a){var n=a.pointers[t];if(n){var r=n.indexOf(i);-1<r&&n.splice(r,1),n.length||delete a.pointers[t],this.cleanupWeakReference(a,e.__rv)}}}},observe:function(e,t,i){var a,n=this,r=this.weakReference(e).callbacks;if(!r[t]){r[t]=[];var o=Object.getOwnPropertyDescriptor(e,t);o&&(o.get||o.set||!o.configurable)||(a=e[t],Object.defineProperty(e,t,{enumerable:!0,get:function(){return a},set:function(i){if(i!==a){n.unobserveMutations(a,e.__rv,t),a=i;var r=n.weakmap[e.__rv];if(r){var o=r.callbacks[t];o&&o.forEach((function(e){e.sync()})),n.observeMutations(i,e.__rv,t)}}}}))}-1===r[t].indexOf(i)&&r[t].push(i),this.observeMutations(e[t],e.__rv,t)},unobserve:function(e,t,i){var a=this.weakmap[e.__rv];if(a){var n=a.callbacks[t];if(n){var r=n.indexOf(i);-1<r&&(n.splice(r,1),n.length||(delete a.callbacks[t],this.unobserveMutations(e[t],e.__rv,t))),this.cleanupWeakReference(a,e.__rv)}}},get:function(e,t){return e[t]},set:function(e,t,i){e[t]=i}},_=function(e){return null!=e?e.toString():void 0};function P(e,t,i){var a=e.el.cloneNode(!0),n=new y(a,t,e.view.options);return n.bind(),e.marker.parentNode.insertBefore(a,i),n}var w={"on-*":{function:!0,priority:1e3,unbind:function(e){this.handler&&e.removeEventListener(this.arg,this.handler)},routine:function(e,t){this.handler&&e.removeEventListener(this.arg,this.handler),this.handler=this.eventHandler(t),e.addEventListener(this.arg,this.handler)}},"each-*":{block:!0,priority:4e3,bind:function(e){this.marker?this.iterated.forEach((function(e){e.bind()})):(this.marker=document.createComment(" tinybind: "+this.type+" "),this.iterated=[],e.parentNode.insertBefore(this.marker,e),e.parentNode.removeChild(e))},unbind:function(e){this.iterated&&this.iterated.forEach((function(e){e.unbind()}))},routine:function(e,t){var i=this,a=this.arg;t=t||[];var n=e.getAttribute("index-property")||"$index";t.forEach((function(e,t){var r={$parent:i.view.models};r[n]=t,r[a]=e;var o=i.iterated[t];if(o)if(o.models[a]!==e){for(var s=void 0,c=void 0,u=t+1;u<i.iterated.length;u++)if((c=i.iterated[u]).models[a]===e){s=u;break}void 0!==s?(i.iterated.splice(s,1),i.marker.parentNode.insertBefore(c.els[0],o.els[0]),c.models[n]=t):c=P(i,r,o.els[0]),i.iterated.splice(t,0,c)}else o.models[n]=t;else{var l=i.marker;i.iterated.length&&(l=i.iterated[i.iterated.length-1].els[0]),o=P(i,r,l.nextSibling),i.iterated.push(o)}})),this.iterated.length>t.length&&function(e,t){for(var i=0;i<e;i++)t()}(this.iterated.length-t.length,(function(){var e=i.iterated.pop();e.unbind(),i.marker.parentNode.removeChild(e.els[0])})),"OPTION"===e.nodeName&&this.view.bindings.forEach((function(e){e.el===i.marker.parentNode&&"value"===e.type&&e.sync()}))},update:function(e){var t=this,i={};Object.keys(e).forEach((function(a){a!==t.arg&&(i[a]=e[a])})),this.iterated.forEach((function(e){e.update(i)}))}},"class-*":function(e,t){var i=" "+e.className+" ";!t==-1<i.indexOf(" "+this.arg+" ")&&(e.className=t?e.className+" "+this.arg:i.replace(" "+this.arg+" "," ").trim())},text:function(e,t){e.textContent=null!=t?t:""},html:function(e,t){e.innerHTML=null!=t?t:""},show:function(e,t){e.style.display=t?"":"none"},hide:function(e,t){e.style.display=t?"none":""},enabled:function(e,t){e.disabled=!t},disabled:function(e,t){e.disabled=!!t},checked:{publishes:!0,priority:2e3,bind:function(e){var t=this;this.callback||(this.callback=function(){t.publish()}),e.addEventListener("change",this.callback)},unbind:function(e){e.removeEventListener("change",this.callback)},routine:function(e,t){"radio"===e.type?e.checked=_(e.value)===_(t):e.checked=!!t}},value:{publishes:!0,priority:3e3,bind:function(e){if(this.isRadio="INPUT"===e.tagName&&"radio"===e.type,!this.isRadio){this.event=e.getAttribute("event-name")||("SELECT"===e.tagName?"change":"input");var t=this;this.callback||(this.callback=function(){t.publish()}),e.addEventListener(this.event,this.callback)}},unbind:function(e){this.isRadio||e.removeEventListener(this.event,this.callback)},routine:function(e,t){if(this.isRadio)e.setAttribute("value",t);else if("select-multiple"===e.type){if(t instanceof Array)for(var i=0;i<e.length;i++){var a=e[i];a.selected=-1<t.indexOf(a.value)}}else _(t)!==_(e.value)&&(e.value=null!=t?t:"")}},if:{block:!0,priority:4e3,bind:function(e){this.marker?!1===this.bound&&this.nested&&this.nested.bind():(this.marker=document.createComment(" tinybind: "+this.type+" "+this.keypath+" "),this.attached=!1,e.parentNode.insertBefore(this.marker,e),e.parentNode.removeChild(e)),this.bound=!0},unbind:function(){this.nested&&(this.nested.unbind(),this.bound=!1)},routine:function(e,t){!!t!==this.attached&&(t?(this.nested||(this.nested=new y(e,this.view.models,this.view.options),this.nested.bind()),this.marker.parentNode.insertBefore(e,this.marker.nextSibling),this.attached=!0):(e.parentNode.removeChild(e),this.attached=!1))},update:function(e){this.nested&&this.nested.update(e)}}};return c.binders=w,c.adapters["."]=E,c.bind=function(i,a,n){var r={};a=a||{},n=n||{},t.forEach((function(e){r[e]=Object.create(null),n[e]&&Object.keys(n[e]).forEach((function(t){r[e][t]=n[e][t]})),Object.keys(c[e]).forEach((function(t){r[e][t]||(r[e][t]=c[e][t])}))})),e.forEach((function(e){var t=n[e];r[e]=null!=t?t:c[e]})),r.starBinders=Object.keys(r.binders).filter((function(e){return 0<e.indexOf("*")})),l.updateOptions(r);var o=new y(i,a,r);return o.bind(),o},c.formatters.negate=c.formatters.not=function(e){return!e},c}));let app,config={},time=0,version=0,toastVisible=!1;document.addEventListener("DOMContentLoaded",(function(){initTabs(),tinybind.formatters.sub=function(e,t){return e-t},tinybind.binders.barwidth=function(e,t){let i=t/config.ledCount*100;e.style.width=i+"%"},tinybind.binders.height=function(e,t){e.style.height=t?"2.666rem":"0.666rem"},tinybind.binders.barpos=function(e,t){let i=(t-1)/config.ledCount*100;e.style.marginLeft=i+"%"},tinybind.formatters.time={read:function(e){let t=Math.floor(e/60);t=String(t).padStart(2,"0");let i=e%60;i=String(i).padStart(2,"0");return t+":"+i},publish:function(e){const t=e.split(":");return m=60*+t[0]+ +t[1],m}},tinybind.formatters.addEmoji=function(e,t){return t+e},tinybind.formatters.replaceUnderscore=function(e){return e.replace(/_/g," ")},tinybind.formatters.formatDate=function(e){if(0!=e){return new Date(1e3*e).toLocaleString()}return""},tinybind.formatters.int={read:function(e){return e},publish:function(e){return parseInt(e)}},tinybind.binders.baroverflow=function(e){let t=document.getElementById("overflow-indicator");e.scrollWidth>e.clientWidth?t.style.boxShadow="-8px 0 4px -3px red inset":t.style.boxShadow=""},getData("time").then((function(e){time=e,getData("version").then((function(e){version=e.toString(),getConfig().then((function(e){config=e})).then((function(){app=tinybind.bind(document.getElementById("app"),{config,version,time,timezones,languages,toggleFirmwareModal,toggleResetModal,loadLanguage,toastVisible})}))}))})),window.setInterval((function(){getData("time").then((function(e){app.models.time=e}))}),1e3),initWatcher()}));