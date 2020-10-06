var gulp = require('gulp'),
    sass = require('gulp-sass'),
    connect = require('gulp-connect'),
    plumber = require('gulp-plumber'),
    rename = require('gulp-rename'),
    terser = require('gulp-terser'),
    concat = require('gulp-concat'),
    pug = require('gulp-pug'),
    i18n = require('gulp-html-i18n');

function reload(done) {
    connect.server({
        livereload: true,
        port: 8080,
        root: 'dist'
    });
    done();
}

function styles() {
    return (
        gulp.src('src/sass/styles.scss')
            .pipe(plumber())
            .pipe(sass.sync({ outputStyle: 'compressed' }))
            .pipe(rename('styles.min.css'))
            .pipe(gulp.dest('dist/assets/css'))
            .pipe(connect.reload())
    );
}

function scripts() {
    return (
        gulp.src('src/js/*.js')
            .pipe(plumber())
            .pipe(terser())
            .pipe(concat('scripts.js'))
            .pipe(gulp.dest('dist/assets/js'))
            .pipe(connect.reload())
    );
}

function html() {
    return (
        gulp.src('*.html')
            .pipe(plumber())
            .pipe(connect.reload())
    );
}

// .pipe(pug({
//     i18n: {
//         dest: 'dist',
//         locales: 'src/locales/*.json',
//         localeExtension: true
//     },
//     pretty: false
// }))
function views() {
    return (
        gulp.src('src/pug/pages/*.pug')
            .pipe(plumber())
            .pipe(pug())
            .pipe(i18n({
                langDir: 'src/locales',
                delimiters: ['${', '}']

            }))
            .pipe(gulp.dest('dist'))
            .pipe(connect.reload())
    )
}

function watchTask(done) {
    gulp.watch('*.html', html);
    gulp.watch('src/sass/**/*.scss', styles);
    gulp.watch('src/js/*.js', scripts);
    gulp.watch('src/pug/**/*.pug', views);
    gulp.watch('src/locales/**/*.json', views);
    done();
}

const watch = gulp.parallel(watchTask, reload);
const build = gulp.series(gulp.parallel(styles, scripts, html, views));

exports.reload = reload;
exports.styles = styles;
exports.scripts = scripts;
exports.html = html;
exports.views = views;
exports.watch = watch;
exports.build = build;
exports.default = watch;