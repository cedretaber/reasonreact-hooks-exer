module Book = Books_Entities.Book

type t =
    FetchBooks
  | BooksFetched of Book.t list
  | FetchBook of Book.id
  | BookFetched of Book.t
  | CreateBook of string * string
  | UpdateBook of Book.id * string * string
  | DeleteBook of Book.id
  | ChangeTitle of string
  | ChangeAuthor of string
  | ClearForm
  | MoveToIndex
  | MoveToNewBook
  | MoveToBook of Book.id