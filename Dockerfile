FROM circleci/node:11

RUN sudo apt-get update \
    && sudo apt-get install -y ocaml-nox \
    && sudo wget http://kr.archive.ubuntu.com/ubuntu/pool/main/b/bubblewrap/bubblewrap_0.3.1-4_amd64.deb \
    && sudo dpkg -i bubblewrap_0.3.1-4_amd64.deb \
    && sudo wget https://github.com/ocaml/opam/releases/download/2.0.4/opam-2.0.4-x86_64-linux \
    && sudo cp opam-2.0.4-x86_64-linux /usr/local/bin/opam \
    && sudo chmod a+x /usr/local/bin/opam \
    && opam init --disable-sandboxing -y \
    && eval $(opam env) \
    && opam switch create ocaml-base-compiler.4.02.3 -y \
    && opam switch ocaml-base-compiler.4.02.3

CMD ["node"]