module Action = Books_Action
open! Action

type t = {
  books: Book.t list;
  title: string;
  author: string
}

let empty = { books= []; title= ""; author= "" }

let reducer state = function
    BooksFetched books -> { state with books }
  | BookFetched {Book.title; author} -> { state with title; author }
  | ChangeTitle title -> { state with title }
  | ChangeAuthor author -> { state with author }
  | ClearForm -> { state with title= ""; author= "" }
  | _ -> state