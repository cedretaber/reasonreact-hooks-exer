
module Make(Action : sig
                       type t
                     end)
            (State : sig
                       type t
                       val empty : t
                       val reducer : t -> Action.t -> t
                     end) = struct

  let context : (State.t * (Action.t -> unit)) React.Context.t =
    React.createContext (State.empty, fun _ -> ())

  module Provider = struct
    let make = React.Context.provider context

    external makeProps :
      value:'a ->
      children:React.element ->
      ?key:string ->
      unit ->
      < value :'a ; children : React.element > Js.t = "" [@@bs.obj]
  end

  let useStore (action_creator : (Action.t -> unit) -> State.t -> Action.t -> unit) initial_state =
    let (state, dispatch) = React.useReducer State.reducer initial_state in
    state, action_creator dispatch state

  let useStoreWithoutSideEffects state =
    useStore (fun dispatch _state action -> dispatch action) state

  let useStateAndAction () =
    React.useContext context

  let useState () =
    let (state, _) = useStateAndAction () in state

  let useAction () =
    let (_, action) = useStateAndAction () in action
end