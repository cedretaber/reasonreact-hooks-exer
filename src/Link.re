[@react.component]
let make = (~href, ~children) => {
  let onClick = event => {
    event->ReactEvent.Mouse.preventDefault;
    ReasonReactRouter.push(href);
  };
  <a href onClick >
    children
  </a>
}