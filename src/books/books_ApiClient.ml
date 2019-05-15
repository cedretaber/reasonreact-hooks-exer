[@@@warning "-6"]

module Book = Books_Entities.Book

open Util

module type Interface = sig
  val fetch_books : unit -> Book.t list Promise.t
  val fetch_book : Book.id -> Book.t Promise.t
  val create_book : string -> string -> unit Promise.t
  val update_book : Book.id -> string -> string -> unit Promise.t
  val delete_book : Book.id -> unit Promise.t
end

module Default : Interface = struct
  let fetch_books () =
    let open Promise in
    let%bind res = Fetch.fetch "/api/books" in
    let%bind json = Fetch.Response.json res in
    return (
      json
      |> Js.Json.decodeArray
      |> Option.get_or_else [||]
      |> Array.to_list
      |> List.map (fun json ->
        match (
          let open! Option in
          let%bind obj = Js.Json.decodeObject json in
          let%bind id = Js.Dict.get obj "id" |> bind (fun id -> map int_of_float @@ Js.Json.decodeNumber id)
          and title = Js.Dict.get obj "title" |> bind Js.Json.decodeString
          and author = Js.Dict.get obj "author" |> bind Js.Json.decodeString in
          return Book.{ id; title; author }
        ) with
            Some book -> [book]
          | None -> []
      )
      |> List.concat
    )

  let fetch_book id =
    let id = Book.id_to_string id in
    let open Promise in
    let%bind res = Fetch.fetch {j|/api/books/$(id)|j} in
    let%bind json = Fetch.Response.json res in
    return @@ Belt.Option.getExn (
      let open! Option in
      let%bind dict = Js.Json.decodeObject json in
      let%bind id = Js.Dict.get dict "id" |> Option.bind (fun id -> Js.Json.decodeNumber id |> Option.map int_of_float)
      and title = Js.Dict.get dict "title" |> Option.bind Js.Json.decodeString
      and author = Js.Dict.get dict "author" |> Option.bind Js.Json.decodeString in
      return Book.{id; title; author}
    )

  let create_book title author =
    let payload = Js.Dict.fromList [
      "title", Js.Json.string title;
      "author", Js.Json.string author
    ] in
    Fetch.fetchWithInit "/api/books" (
      Fetch.RequestInit.make
        ~method_:Post
        ~body:(Fetch.BodyInit.make @@ Js.Json.stringify @@ Js.Json.object_ payload)
        ~headers:(Fetch.HeadersInit.makeWithDict @@ Js.Dict.fromList ["Content-Type", "application/json"])
        ())
    |> Promise.ignore

  let update_book id title author =
    let id = Book.id_to_string id in
    let payload = Js.Dict.fromList [
      "title", Js.Json.string title;
      "author", Js.Json.string author
    ] in
    Fetch.fetchWithInit {j|/api/books/$(id)|j} (
      Fetch.RequestInit.make
        ~method_:Put
        ~body:(Fetch.BodyInit.make @@ Js.Json.stringify @@ Js.Json.object_ payload)
        ~headers:(Fetch.HeadersInit.makeWithDict @@ Js.Dict.fromList ["Content-Type", "application/json"])
        ())
    |> Promise.ignore

  let delete_book id =
    let id = Book.id_to_string id in
    Fetch.fetchWithInit {j|/api/books/$(id)|j} (
      Fetch.RequestInit.make ~method_:Delete ())
    |> Promise.ignore
end