
module Book = struct

  type id = int

  let id_to_string = string_of_int

  let id_of_string = int_of_string

  type t = {
    id: id;
    title: string;
    author: string
  }
end