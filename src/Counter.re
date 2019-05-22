
module Notice = {
  [@react.component]
  let make = (~message, ~clear) => {
    <div className="notification">
      <button className="delete" onClick={clear} />
      <p>{React.string(message)}</p>
    </div>
  }
}

module Counter = {
  [@react.component]
  let make = (~count) => {
    <div>
      <span>{React.string("Count: " ++ string_of_int(count))}</span>
    </div>
  }
};

module Button = {
  [@react.component]
  let make = (~onClick, ~active) => {
    <button className="button" onClick disabled={!active}>
      {React.string("Click!")}
    </button>
  }
}

type counter = Count1 | Count2;

type action =
  | CountUp(counter)
  | Activate
  | Deactivate
  | RaiseError(string)
  | ClearError;

type state = {
  count1: int,
  count2: int,
  active: bool,
  errorMessage: option(string)
};

let reducer = (state, action) => switch (action) {
| CountUp(Count1) => { ...state, count1: state.count1 + 1 }
| CountUp(Count2) => { ...state, count2: state.count2 + 1 }
| Activate => { ...state, active: true }
| Deactivate => { ...state, active: false }
| RaiseError(message) => { ...state, errorMessage: Some(message)}
| ClearError => { ...state, errorMessage: None }
};

class actionCreator(dispatch) = {
  pub clickButton = () => {
    dispatch(Deactivate);
    Js.Global.setTimeout(() => {
      if (Random.int(10) < 3) {
        dispatch(RaiseError("Some error was occurred!"));
      } else {
        dispatch(CountUp(Count2));
      }
      dispatch(Activate);
    }, 500) |> ignore;
  };

  pub countUp = () => {
    dispatch(CountUp(Count1));
  };

  pub clearNotice = () => {
    dispatch(ClearError);
  }
};

[@react.component]
let make = () => {
  let ({ count1, count2, active, errorMessage }, dispatch) = React.useReducer(
    reducer,
    { count1: 0, count2: 0, active: true, errorMessage: None }
  );
  let actionCreator = (new actionCreator)(dispatch);
  let notice = switch (errorMessage) {
  | Some(message) =>
    let clear = (e) => {
      e->ReactEvent.Mouse.preventDefault;
      actionCreator#clearNotice();
    };
    <Notice message clear />
  | None =>
    React.null
  };
  let onClick = (e) => {
    e->ReactEvent.Mouse.preventDefault;
    actionCreator#clickButton();
  };
  React.useEffect0(_ => {
    Random.self_init();
    let iid = Js.Global.setInterval(() => {
      actionCreator#countUp();
    }, 1000);
    Some(_ => {
      Js.Global.clearInterval(iid);
    });
  });
  <>
    {notice}
    <div className="card">
      <div className="card-content">
        <Counter count={count1} />
        <Counter count={count2} />
      </div>
      <footer className="card-footer">
        <Button onClick active />
      </footer>
    </div>
    <Link href="/">{React.string("back")}</Link>
  </>
}