module Option = struct
  let return x = Some x

  let map f = function
      Some x -> Some (f x)
    | None -> None

  let bind f = function
      Some x -> f x
    | None -> None

  let both x y = match x, y with
      (Some x, Some y) -> Some (x, y)
    | _ -> None

  module Let_syntax = struct
    let return = return
    let bind x ~f = bind f x
    let map x ~f = map f x
    let both = both
    
    module Open_on_rhs = struct
      let return = return
    end
  end

  let get_or_else default = function
      Some x -> x
    | None -> default
end