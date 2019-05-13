
module Book = Books_Entities.Book

module Option = struct
  let flat_map f = function
      None -> None
    | Some a -> f a

  let map f = flat_map @@ fun x -> Some (f x)
end

module Promise = struct
  include Js.Promise

  let ignore promise = then_ (fun _ -> resolve ()) promise
end

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
    Fetch.fetch "/api/books"
    |> then_ Fetch.Response.json
    |> then_ (fun json -> Js.Json.decodeArray json |> resolve)
    |> then_ (fun opt ->
      Belt.Option.getWithDefault opt [||]
      |> Array.to_list
      |> List.map (fun json ->
        match Js.Json.decodeObject json with
            None -> []
          | Some obj ->
          match (
            (Js.Dict.get obj "id" |> Option.flat_map @@ fun id -> Option.map int_of_float @@ Js.Json.decodeNumber id),
            (Js.Dict.get obj "title" |> Option.flat_map Js.Json.decodeString),
            (Js.Dict.get obj "author" |> Option.flat_map Js.Json.decodeString)
          ) with
              (Some id, Some title, Some author) -> [Book.{ id; title; author }]
            | _ -> []
      )
      |> List.concat
      |> resolve
    )

  let fetch_book id =
    let id = Book.id_to_string id in
    let open Promise in
    Fetch.fetch {j|/api/books/$(id)|j}
    |> then_ Fetch.Response.json
    |> then_ (fun json ->
      let dict = Belt.Option.getExn @@ Js.Json.decodeObject json in
      let id = Belt.Option.getExn (Js.Dict.get dict "id" |> Option.flat_map (fun id -> Js.Json.decodeNumber id |> Option.map int_of_float)) in
      let title = Belt.Option.getExn (Js.Dict.get dict "title" |> Option.flat_map Js.Json.decodeString) in
      let author = Belt.Option.getExn (Js.Dict.get dict "author" |> Option.flat_map Js.Json.decodeString) in
      resolve Book.{id; title; author}
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