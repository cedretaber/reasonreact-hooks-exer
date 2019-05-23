
let s = React.string;

module Action = Books_Action;
module State = Books_State;
module BooksStore = Books_Store;
module Book = Books_Entities.Book;

module BookTable = {
  module BookRow = {
    [@react.component]
    let make = (~book as {Book.id, title, author}) => {
      let action = BooksStore.useAction();
      let delete = React.useCallback0(event => {
        event->ReactEvent.Mouse.preventDefault;
        action(Action.DeleteBook(id));
      });
      <tr>
        <td>id->Book.id_to_string->s</td>
        <td>title->s</td>
        <td>author->s</td>
        <td>
          <Link href={j|/books/$(id)|j}>
            "Edit"->s
          </Link>
        </td>
        <td>
          <a href="#" onClick=delete>"x"->s</a>
        </td>
      </tr>
    }
  };

  [@react.component]
  let make = () => {
    let {State.books} = BooksStore.useState();
    let books =
      books
      |> List.mapi(
        (idx, book) => <BookRow key={j|bookList__bookRow-$(idx)|j} book />
      )
      |> Array.of_list;
    <table className="table">
      <thead>
        <tr>
          <th>"#"->s</th>
          <th>"Title"->s</th>
          <th>"Author"->s</th>
          <th />
          <th />
        </tr>
      </thead>
      <tbody>
        books->React.array
      </tbody>
    </table>
  };
};

[@react.component]
let make = () => {
  let action = BooksStore.useAction();
  React.useEffect0(_ => {
    action(Action.FetchBooks);
    None
  });
  <>
    <Link href="/books/new">"new"->s</Link>
    <BookTable />
    <Link href="/">"Back"->s</Link>
  </>
}