
let s = React.string;

module Books = Books_Books;
module Book = Books_Entities.Book;

module BookTable = {
  module BookRow = {
    [@react.component]
    let make = (~book as {Book.id, title, author}, ~actionCreator) => {
      let delete = event => {
        event->ReactEvent.Mouse.preventDefault;
        actionCreator#delete_book(id);
      };
      <tr>
        <td>{s(Book.id_to_string(id))}</td>
        <td>{s(title)}</td>
        <td>{s(author)}</td>
        <td>
          <Link href={{j|/books/$(id)|j}}>
            {s("Edit")}
          </Link>
        </td>
        <td>
          <a href="#" onClick={delete}>{s("x")}</a>
        </td>
      </tr>
    }
  };

  [@react.component]
  let make = (~books, ~actionCreator) => {
    let books =
      books
      |> List.mapi(
        (idx, book) => <BookRow key={{j|bookList__bookRow-$(idx)|j}} book={book} actionCreator={actionCreator} />
      )
      |> Array.of_list;
    <table className="table">
      <thead>
        <tr>
          <th>{s("#")}</th>
          <th>{s("Title")}</th>
          <th>{s("Author")}</th>
          <th />
          <th />
        </tr>
      </thead>
      <tbody>
        {React.array(books)}
      </tbody>
    </table>
  };
};

[@react.component]
let make = (~books, ~actionCreator) => {
  React.useEffect0(_ => {
    actionCreator#fetch_books();
    None
  });
  <>
    <Link href="/books/new">{s("new")}</Link>
    <BookTable books={books} actionCreator={actionCreator} />
    <Link href="/">{s("Back")}</Link>
  </>
}