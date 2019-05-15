module Promise = struct
  include Js.Promise

  external then2_ : ('a -> 'b [@bs.uncurry]) -> 'b t = "then" [@@bs.send.pipe: 'a t]

  let return x = resolve x

  let map f x = then2_ f x

  let bind f x = then_ f x

  let both x y = all2 (x, y)

  module Let_syntax = struct
    let return = return
    let bind x ~f = bind f x
    let map x ~f = map f x
    let both = both
    
    module Open_on_rhs = struct
      let return = return
    end
  end

  let ignore promise = then_ (fun _ -> resolve ()) promise
end