module Make =
       (
         Action: {
           type t;
         },
         State: {
           type t;
           let empty: t;
           let reducer: t => Action.t => t;
         },
       ) => {
  let context: React.Context.t((State.t, Action.t => unit)) =
    React.createContext((State.empty, _ => ()));

  module Provider = {
    let make = React.Context.provider(context);

    [@bs.obj]
    external makeProps:
      (~value: 'a, ~children: React.element, ~key: string=?, unit) =>
      { .
        "value": 'a,
        "children": React.element,
      } = "";
  };

  let useStateAndAction = () => React.useContext(context);

  let useState = () => {
    let (state, _) = useStateAndAction();
    state;
  };

  let useAction = () => {
    let (_, action) = useStateAndAction();
    action;
  };

  [@react.component]
  let make = (~actionCreator: (Action.t => unit) => Action.t => unit, ~initialState=?, ~children) => {
    let initialState =
      switch (initialState) {
      | Some(initialState) => initialState
      | None => State.empty
      };
    let (state, dispatch) = React.useReducer(State.reducer, initialState);
    let value = (state, actionCreator(dispatch));
    <Provider value>
      children
    </Provider>
  };
};
