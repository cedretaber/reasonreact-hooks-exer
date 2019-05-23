
module Notice = {
  [@react.component]
  let make = (~message, ~clear) => {
    <div className="notification">
      <button className="delete" onClick=clear>"x"->React.string</button>
      <p>message->React.string</p>
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
      "Click!"->React.string
    </button>
  }
};

[@react.component]
let make = () => {
  let (counter1, setCounter1) = React.useState(() => 0);
  let (counter2, setCounter2) = React.useState(() => 0);
  let (active, setActive) = React.useState(() => true);
  let (errorMessage, setErrorMessage) = React.useState(() => None);

  let clear = React.useCallback0(e => {
    e->ReactEvent.Mouse.preventDefault;
    setErrorMessage(_ => None);
  });

  let notice = switch (errorMessage) {
  | Some(message) =>
    <Notice message clear />
  | None =>
    React.null
  };

  let onClick = React.useCallback0(e => {
    e->ReactEvent.Mouse.preventDefault;
    setActive(_ => false);
    Js.Global.setTimeout(() => {
      if (Random.int(10) < 3) {
        setErrorMessage(_ => Some("Some error was occurred!"));
      } else {
        setCounter2(i => i + 1);
      }
      setActive(_ => true);
    }, 500) |> ignore;
  });

  React.useEffect0(_ => {
    Random.self_init();
    let iid = Js.Global.setInterval(() => {
      setCounter1(i => i + 1);
    }, 1000);
    Some(_ => {
      Js.Global.clearInterval(iid);
    });
  });

  <>
    notice
    <div className="card">
      <div className="card-content">
        <Counter count=counter1 />
        <Counter count=counter2 />
      </div>
      <footer className="card-footer">
        <Button onClick active />
      </footer>
    </div>
    <Link href="/">"back"->React.string</Link>
  </>
}