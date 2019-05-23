
open Util

open Books_Action
open! Books_State

module ApiClient = Books_ApiClient

let move_to_index () = ReasonReactRouter.push "/books/" |> ignore
let move_to_newbook () = ReasonReactRouter.push "/books/new/" |> ignore
let move_to_book id = ReasonReactRouter.push {j|/books/$(id)/|j} |> ignore

let make (module Client : ApiClient.Interface) (dispatch : Action.t -> unit) = function
    FetchBooks ->
    Client.fetch_books ()
    |> Promise.iter (fun books ->
      dispatch (BooksFetched books)
    )
  | FetchBook id ->
      Client.fetch_book id
      |> Promise.iter (fun {Book.title; author} ->
        (dispatch (BookFetched Book.{id; title; author}) [@warning "-45"])
      )
  | CreateBook (title, author) ->
    Client.create_book title author
    |> Promise.iter (fun _ -> move_to_index ())
  | UpdateBook (id, title, author) ->
    Promise.(
      let%bind _ = Client.update_book id title author in
      let%bind books = Client.fetch_books () in
      dispatch (BooksFetched books);
      return @@ move_to_index ()
    ) [@warning "-6"] |> ignore
  | DeleteBook id ->
    Promise.(
      let%bind _ = Client.delete_book id in
      let%bind books = Client.fetch_books () in
      return @@ dispatch (BooksFetched books)
    ) [@warning "-6"] |> ignore;
  | MoveToIndex ->
    move_to_index ()
  | MoveToNewBook ->
    move_to_newbook ()
  | MoveToBook id ->
    move_to_book id
  | action ->
    dispatch action