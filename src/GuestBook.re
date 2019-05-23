
let s = React.string;

module Entity = {
  module Post = {
    type t = {
      name: string,
      text: string,
      time: Js.Date.t
    };
  };
};

type state = {
  name: string,
  text: string,
  postList: list(Entity.Post.t)
};

type action =
  | ChangeName(string)
  | ChangeText(string)
  | OnPost;

module PostForm = {
  module Input = {
    [@react.component]
    let make = (~value, ~placeholder, ~onChange) => {
      <input className="input" type_="text" value placeholder onChange />
    }
  };

  module Button = {
    [@react.component]
    let make = (~value, ~onClick) => {
      <button className="button" onClick >value->s</button>
    }
  };

  [@react.component]
  let make = (~nameValue, ~textValue, ~dispatch) => {
    let onNameChange = event => {
      dispatch(ChangeName(event->ReactEvent.Form.target##value))
    };
    let onTextChange = event => {
      dispatch(ChangeText(event->ReactEvent.Form.target##value))
    };
    let postPost = event => {
      event->ReactEvent.Mouse.preventDefault;
      dispatch(OnPost)
    };
    <form>
      <div className="field">
        <div className="control">
          <Input value=nameValue placeholder={j|お名前|j} onChange=onNameChange />
        </div>
      </div>
      <div className="field">
        <div className="control">
          <Input value=textValue placeholder={j|一言|j} onChange=onTextChange />
        </div>
      </div>
      <div className="field">
        <div className="control">
          <Button value={j|投稿|j} onClick=postPost />
        </div>
      </div>
    </form>
  }
};

[@bs.send]
external toLocaleString : Js.Date.t => string => string = "";

module PostList = {
  module Post = {
    [@react.component]
    let make = (~post as {Entity.Post.text, name, time}) => {
      <div className="card" style={
        ReactDOMRe.Style.make(~margin="1rem 1rem 0", ())
      }>
        <header className="card-header">
          <p className="card-header-title">
            name->s
          </p>
        </header>
        <div className="card-content">
          <div className="content">
            text->s
            <br />
          </div>
        </div>
        <footer className="card-footer">
          <div className="card-footer-item has-text-grey-light">
            <time>
              {time->toLocaleString("ja-JP")->s}
            </time>
          </div>
        </footer>
      </div>
    }
  };

  [@react.component]
  let make = (~postList) => {
    let postList =
      postList
      |> List.mapi((idx, post) => {
        <Post key={{j|postList-post-$(idx)|j}} post={post} />
      })
      |> Array.of_list;
    <div>
      postList->React.array
    </div>
  }
};

let initialState = {
  name: "",
  text: "",
  postList: []
};

[@react.component]
let make = () => {
  let ({name, text, postList}, dispatch) = React.useReducer(
    (state, action) => switch (action) {
    | ChangeName(name) => {...state, name}
    | ChangeText(text) => {...state, text}
    | OnPost =>
      let {name, text, postList} = state;
      let post = Entity.Post.{text, name, time: Js.Date.make()};
      if (name != "" && text != "") {
        {name: "", text: "", postList: [post, ...postList]}
      } else {
        state
      }
    },
    initialState
  );
  <div>
    <div className="level">
      <div className="level-left">
        <div className="level-item">
          <PostForm nameValue=name textValue=text dispatch />
        </div>
      </div>
    </div>
    <PostList postList />
    <Link href="/">"Back"->React.string</Link>
  </div>
}