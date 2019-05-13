
let s = React.string;

module Books = Books_Books;

module Input = {
  [@react.component]
  let make = (~value, ~onChange) => {
    <input className="input" type_="text" value onChange />
  }
};

module Button = {
  [@react.component]
  let make = (~value, ~onClick) => {
    <button className="button" onClick >(s(value))</button>
  }
};

[@react.component]
let make = (~id=?, ~actionCreator) => {
  let (title, setTitle) = React.useState(_ => "");
  let (author, setAuthor) = React.useState(_ => "");
  React.useEffect0(() => {
    switch (id) {
    | None => ()
    | Some(id) =>
      actionCreator#fetch_book(id, (title, author) => {
        setTitle(_ => title);
        setAuthor(_ => author);
      })
    };
    None
  })
  let changeTitle = event => {
    let title = event->ReactEvent.Form.target##value;
    setTitle(_ => title);
  };
  let changeAuthor = event => {
    let author = event->ReactEvent.Form.target##value;
    setAuthor(_ => author);
  };
  let submit = event => {
    event->ReactEvent.Mouse.preventDefault;
    switch (id) {
    | None => actionCreator#create_book(title, author)
    | Some(id) => actionCreator#update_book(id, title, author)
    }
  };
  <>
    <form>
      <div className="field">
        <label className="label">{s("Title")}</label>
        <div className="control">
          <Input value={title} onChange={changeTitle} />
        </div>
      </div>
      <div className="field">
        <label className="label">{s("Author")}</label>
        <div className="control">
          <Input value={author} onChange={changeAuthor} />
        </div>
      </div>
      <div className="field">
        <div className="control">
          <Button value={Belt.Option.isSome(id) ? "Update" : "Create"} onClick={submit} />
        </div>
      </div>
    </form>
    <Link href="/books">{s("Back")}</Link>
  </>
}