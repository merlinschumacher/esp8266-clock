var gulp = require('gulp'),
    sass = require('gulp-sass'),
    connect = require('gulp-connect'),
    plumber = require('gulp-plumber'),
    rename = require('gulp-rename'),
    terser = require('gulp-terser'),
    concat = require('gulp-concat'),
    pug = require('gulp-pug'),
    purgecss = require('gulp-purgecss'),
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
            .pipe(sass.sync({
                outputStyle: 'compressed',
                includePaths: [
                    'node_modules/spectre.css/src'
                ]
            }))
            .pipe(purgecss({
                content: ['dist/*.html'],
                fontFace: true,
                keyframes: true,
                safelist: ['badge', 'fadeout']

            }))
            .pipe(rename('styles.css'))
            .pipe(gulp.dest('dist/'))
            .pipe(connect.reload())
    );
}

function scripts() {
    return (
        gulp.src('src/js/*.js')
            .pipe(plumber())
            .pipe(concat('scripts.js'))
            .pipe(terser({
                ecma: 2016
            }))
            .pipe(gulp.dest('dist/'))
            .pipe(connect.reload())
    );
}

function views() {
    return (
        gulp.src('src/pug/pages/*.pug')
            .pipe(plumber())
            .pipe(pug())
            .pipe(i18n({
                langDir: './src/locales',
                defaultLang: 'en',
                fallback: 'en',
            }))
            .pipe(gulp.dest('dist'))
            .pipe(connect.reload())
    )
}

function watchTask(done) {
    gulp.watch('src/sass/**/*.scss', styles);
    gulp.watch('src/js/*.js', scripts);
    gulp.watch('src/pug/**/*.pug', views);
    gulp.watch('src/pug/**/*.pug', styles);
    gulp.watch('src/locales/**/*.json', views);
    done();
}

const watch = gulp.parallel(watchTask, reload);
const build = gulp.series(gulp.parallel(views, scripts, styles));

exports.reload = reload;
exports.styles = styles;
exports.scripts = scripts;
exports.views = views;
exports.watch = watch;
exports.build = build;
exports.default = watch;