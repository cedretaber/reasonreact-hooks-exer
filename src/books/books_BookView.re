
let s = React.string;

module Action = Books_Action;
module State = Books_State;
module BooksStore = Books_Store;

module Input = {
  [@react.component]
  let make = (~value, ~onChange) => {
    <input className="input" type_="text" value onChange />
  }
};

module Button = {
  [@react.component]
  let make = (~value, ~onClick) => {
    <button className="button" onClick >value->s</button>
  }
};

[@react.component]
let make = (~id=?) => {
  let ({State.title, author}, action) = BooksStore.useStateAndAction();
  React.useEffect0(() => {
    switch (id) {
    | None =>
      action(Action.ClearForm)
    | Some(id) =>
      action(Action.FetchBook(id))
    };
    None
  });
  let changeTitle = React.useCallback0(event => {
    let title = event->ReactEvent.Form.target##value;
    action(Action.ChangeTitle(title));
  });
  let changeAuthor = React.useCallback0(event => {
    let author = event->ReactEvent.Form.target##value;
    action(Action.ChangeAuthor(author));
  });
  let submit = React.useCallback2(event => {
    event->ReactEvent.Mouse.preventDefault;
    switch (id) {
    | None => action(Action.CreateBook (title, author))
    | Some(id) => action(Action.UpdateBook(id, title, author))
    }
  }, (title, author));
  <>
    <form>
      <div className="field">
        <label className="label">"Title"->s</label>
        <div className="control">
          <Input value=title onChange=changeTitle />
        </div>
      </div>
      <div className="field">
        <label className="label">"Author"->s</label>
        <div className="control">
          <Input value=author onChange=changeAuthor />
        </div>
      </div>
      <div className="field">
        <div className="control">
          <Button value={Belt.Option.isSome(id) ? "Update" : "Create"} onClick=submit />
        </div>
      </div>
    </form>
    <Link href="/books">"Back"->s</Link>
  </>
}