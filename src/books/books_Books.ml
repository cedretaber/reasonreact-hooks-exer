
module Book = Books_Entities.Book
module ApiClient = Books_ApiClient

open Util

type state = {
  books: Book.t list
}

type action =
    BooksFetched of Book.t list
  
module ActionCreator = struct
  let make (module Client : ApiClient.Interface) dispatch =
    object (self)
      method fetch_books =
        Client.fetch_books ()
        |> Promise.iter (fun books ->
          dispatch (BooksFetched books)
        )

      method fetch_book id (update : string -> string -> unit) =
        Client.fetch_book id
        |> Promise.iter (fun {Book.title; author} ->
          update title author
        )

      method move_to_index =
        ReasonReactRouter.push "/books/"
        
      method move_to_new_book =
        ReasonReactRouter.push "/books/new/"

      method move_to_book id =
        ReasonReactRouter.push {j|/books/$(id)/|j}

      method delete_book id =
        Client.delete_book id |> ignore;
        self#fetch_books;
        self#move_to_index

      method create_book title author =
        Client.create_book title author |> ignore;
        self#move_to_index

      method update_book id title author =
        Client.update_book id title author |> ignore;
        self#move_to_index
    end
end

let reducer _state = function
    BooksFetched books -> { books }