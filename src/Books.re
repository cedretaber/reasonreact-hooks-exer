module Books = Books_Books;

module Book = Books_Entities.Book;

module ApiClient = Books_ApiClient

module BooksView = Books_BooksView;

module BookView = Books_BookView;

let initialState = Books.{
  books: []
};

module ActionCreator = Books.ActionCreator(ApiClient.Default);

[@react.component]
let make = () => {
  let ({Books.books}, dispatch) = React.useReducer(
    Books.reducer,
    initialState
  );
  let actionCreator = ActionCreator.make(dispatch);
  let url = ReasonReactRouter.useUrl();
  switch (url.path) {
  | ["books", "new"] =>
    <BookView actionCreator={actionCreator} />
  | ["books", id] =>
    <BookView id={Book.id_of_string(id)} actionCreator={actionCreator} />
  | _ =>
    <BooksView books={books} actionCreator={actionCreator} />
  };
}