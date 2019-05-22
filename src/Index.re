module List = {
  [@react.component]
  let make = () => {
    <ul>
      <li><Link href={"/guestbook"} >{React.string("Guest Book")}</Link></li>
      <li><Link href={"/books"}>{React.string("Book List")}</Link></li>
      <li><Link href={"/modal"}>{React.string("Modal")}</Link></li>
      <li><Link href={"/counter"}>{React.string("Counter")}</Link></li>
    </ul>
  }
}

module App = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    let content = switch (url.path) {
    | ["guestbook"] => <GuestBook />
    | ["books", ..._] => <Books />
    | ["modal", ..._] => <Modal />
    | ["counter", ..._] => <Counter />
    | _ => <List />
    };
    <>
      <header className="level" style={
        ReactDOMRe.Style.make(~margin="1rem 1rem 0", ())
      }>
        <div className="level-left">
          <div className="level-item">
            <h1 className="title">
              {React.string("ReasonReact + React Hooks")}
            </h1>
          </div>
        </div>
      </header>
      <hr />
      <div className="container">
        {content}
      </div>
    </>
  }
};

[%%bs.raw {|
  if ('serviceWorker' in navigator) {
    async function testSW() {
      try {
        const reg = await navigator.serviceWorker.register('/sw.js', { scope: "" });
        console.log('Ok. ServiceWorker was registered.', reg);
      } catch (e) {
        console.error("Registration failed.", e);
      }
    }
    testSW();
  }
|}]

ReactDOMRe.renderToElementWithId(<App />, "app");
