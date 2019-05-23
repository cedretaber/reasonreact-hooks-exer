module State = Books_State;
module BooksStore = Books_Store;
module ActionCreator = Books_ActionCreator;
module Book = Books_Entities.Book;
module ApiClient = Books_ApiClient
module BooksView = Books_BooksView;
module BookView = Books_BookView;

[@react.component]
let make = () => {
  let actionCreator = ActionCreator.make(module ApiClient.Default);
  let url = ReasonReactRouter.useUrl();
  <BooksStore actionCreator>
  {
    switch (url.path) {
      | ["books", "new"] =>
        <BookView />
      | ["books", id] =>
        <BookView id=id->Book.id_of_string />
      | _ =>
        <BooksView />
      };
  }
  </BooksStore>
}