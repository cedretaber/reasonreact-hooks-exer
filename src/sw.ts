/// <reference lib="webworker" />

class Book {
  id: number;
  title: string;
  author: string;

  constructor(id: number, title: string, author: string) {
    this.id = id;
    this.title = title;
    this.author = author;
  }
}

class Library {
  books: Book[];
  count: number;

  constructor() {
    this.books = [];
    this.count = 1;
  }

  get booksAsJson() {
    return this.books.map(({ id, title, author }) => {
      return { id, title, author };
    });
  }

  addBook(title: string, author: string) {
    const book = new Book(this.count, title, author);
    this.count += 1;
    this.books.push(book);
  }

  findBook(bookId: number) {
    return this.books.find(({ id }) => id === bookId);
  }

  updateBook(bookId: number, title: string, author: string) {
    const idx = this.books.findIndex(({ id }) => id === bookId);
    if (idx !== undefined) {
      this.books[idx] = new Book(bookId, title, author);
      return true;
    }
    return false;
  }

  deleteBook(bookId: number) {
    const idx = this.books.findIndex(({ id }) => id === bookId);
    if (idx !== undefined) {
      this.books = [...this.books.slice(0, idx), ...this.books.slice(idx + 1)];
      return true;
    }
    return false;
  }
}

const library = new Library();

self.addEventListener("install", _ => {
  console.log("Worker is installing...");
});

self.addEventListener("activate", _ => {
  console.log("Worker is ready!");
});

self.addEventListener("fetch", (event: FetchEvent) => {
  const url = new URL(event.request.url);
  console.log(url.pathname);
  const pathname = url.pathname;
  const method = event.request.method;
  if (pathname === "/api/books") {
    switch (method) {
      case "GET": {
        event.respondWith(
          new Response(JSON.stringify(library.booksAsJson), { status: 200 })
        );
        break;
      }
      case "POST": {
        event.respondWith(
          event.request.json().then(({ title, author }) => {
            library.addBook(title, author);
            return new Response("ok", { status: 200 });
          })
        );
        break;
      }
    }
  } else if (pathname.startsWith("/api/books")) {
    const [_a, _b, _c, idString] = pathname.split("/");
    const id = Number(idString);
    if (isNaN(id)) {
      event.respondWith(new Response("Bad Book ID.", { status: 400 }));
    } else {
      switch (method) {
        case "GET": {
          const book = library.findBook(id);
          if (book === undefined) {
            event.respondWith(new Response("NotFound", { status: 404 }));
          } else {
            event.respondWith(
              new Response(JSON.stringify(book), { status: 200 })
            );
          }
          break;
        }
        case "PUT": {
          event.respondWith(
            event.request.json().then(({ title, author }) => {
              const res = library.updateBook(id, title, author);
              if (res) {
                return new Response("ok", { status: 200 });
              } else {
                return new Response("Bad Book Id", { status: 400 });
              }
            })
          );
          break;
        }
        case "DELETE": {
          const res = library.deleteBook(id);
          if (res) {
            event.respondWith(new Response("ok", { status: 200 }));
          } else {
            event.respondWith(new Response("Bad Book Id", { status: 400 }));
          }
          break;
        }
      }
    }
  }
});
