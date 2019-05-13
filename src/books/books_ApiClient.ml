
module Promise = Js.Promise

module Book = Books_Entities.Book

module Option = struct
  let flat_map f = function
      None -> None
    | Some a -> f a

  let map f = flat_map @@ fun x -> Some (f x)
end

let fetch_books () =
  let open Js.Promise in
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
  let open Js.Promise in
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
  let payload = Js.Dict.empty () in
  Js.Dict.set payload "title" @@ Js.Json.string title;
  Js.Dict.set payload "author" @@ Js.Json.string author;
  Fetch.fetchWithInit "/api/books" @@
    Fetch.RequestInit.make
      ~method_:Post
      ~body:(Fetch.BodyInit.make @@ Js.Json.stringify @@ Js.Json.object_ payload)
      ~headers:(Fetch.HeadersInit.makeWithDict @@ Js.Dict.fromList ["Content-Type", "application/json"])
      ()

let update_book id title author =
  let id = Book.id_to_string id in
  let payload = Js.Dict.empty () in
  Js.Dict.set payload "title" @@ Js.Json.string title;
  Js.Dict.set payload "author" @@ Js.Json.string author;
  Fetch.fetchWithInit {j|/api/books/$(id)|j} @@
    Fetch.RequestInit.make
      ~method_:Put
      ~body:(Fetch.BodyInit.make @@ Js.Json.stringify @@ Js.Json.object_ payload)
      ~headers:(Fetch.HeadersInit.makeWithDict @@ Js.Dict.fromList ["Content-Type", "application/json"])
      ()

let delete_book id =
  let id = Book.id_to_string id in
  Fetch.fetchWithInit {j|/api/books/$(id)|j} @@
    Fetch.RequestInit.make ~method_:Delete ()