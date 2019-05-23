# Hooks Exercise

[![CircleCI](https://circleci.com/gh/cedretaber/reasonreact-hooks-exer.svg?style=svg)](https://circleci.com/gh/cedretaber/reasonreact-hooks-exer)

## Set up ppx_let for BuckleScript

You should have installed OCaml 4.02.3 and ppx_let.

```
$ pwd
path/to/hooks-exer

$ mkdir ppx_let

$ cd ppx_let

$ yarn init

$ opam switch 4.02.3

$ ln -s `opam config var ppx_let:lib`/ppx ppx

$ yarn link

$ cd ..

$ yarn link ppx_let
```

## Build and Watch

```
$ yarn build

$ yarn webpack
```

## Server start

```
$ yarn server
```

## Test

```
$ yarn test
```