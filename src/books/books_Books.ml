
module Promise = struct
  include Js.Promise

  external then2_ : ('a -> 'b [@bs.uncurry]) -> 'b t = "then" [@@bs.send.pipe: 'a t]
end

module Book = Books_Entities.Book
module ApiClient = Books_ApiClient

type state = {
  books: Book.t list
}

type action =
    BooksFetched of Book.t list

class action_creator dispatch =
  let move_to_index () = ReasonReactRouter.push("/books/") in
  let move_to_new_book () = ReasonReactRouter.push("/books/new/") in
  let move_to_book id = ReasonReactRouter.push("/books/" ^ string_of_int id ^ "/") in
  object (self)
    method fetch_books () =
      ApiClient.fetch_books ()
      |> Promise.then2_ (fun books ->
        dispatch (BooksFetched books)
      )
      |> ignore

    method fetch_book id (update: string -> string -> unit) =
      ApiClient.fetch_book id
      |> Promise.then2_ (fun {Book.title; author} ->
        update title author
      )
      |> ignore

    method move_to_index =
      move_to_index ()
      
    method move_to_new_book =
      move_to_new_book ()

    method move_to_book id =
      move_to_book id

    method delete_book id =
      ApiClient.delete_book id |> ignore;
      self#fetch_books();
      move_to_index ()

    method create_book title author =
      ApiClient.create_book title author |> ignore;
      move_to_index ()

    method update_book id title author =
      ApiClient.update_book id title author |> ignore;
      move_to_index ()
  end

let reducer _state = function
    BooksFetched books -> { books }