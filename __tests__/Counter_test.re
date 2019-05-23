open Jest;
open Expect;
open ReactTestingLibrary;

module CCounter = Counter;
open! Counter;

[@bs.get] external firstChild : Dom.element => Dom.node = "";
[@bs.get] external innerHTML : Dom.node => string = "";
[@bs.send] external querySelector : Dom.element => string => Dom.node = "";
let querySelector(query, dom) = querySelector(dom, query);

describe("Notice", () => {
  test("render", () => {
    let testMessage = "test message";
    <Notice message=testMessage clear={_ => ()} />
    |> render
    |> expect
    |> toMatchSnapshot;
  });

  test("close", () => {
    let c = ref(0);
    let el = render(<Notice message="text" clear={_ => c := c^ + 1} />);

    let button = el |> getByText(~matcher=`Str("x"));
    button->FireEvent.click;

    c^ |> expect |> toBe(1);
  });
});

describe("Counter", () => {
  test("render", () => {
    <Counter count=42 />
    |> render
    |> expect
    |> toMatchSnapshot;
  });

  test("currect count", () => {
    let count = 42;
    <Counter count />
    |> render
    |> container
    |> querySelector("span")
    |> innerHTML
    |> expect
    |> toEqual({j|Count: $count|j});
  });
});

describe("Button", () => {
  test("render", () => {
    <Button onClick={_ => ()} active=true />
    |> render
    |> expect
    |> toMatchSnapshot;
  });

  test("click button", () => {
    let c = ref(0);
    let el = render(<Button onClick={_ => c := c^ + 1} active=true />);

    let button = el |> getByText(~matcher=`Str("Click!"));
    button->FireEvent.click;

    c^ |> expect |> toBe(1);
  });

  test("click disabled button", () => {
    let c = ref(0);
    let el = render(<Button onClick={_ => c := c^ + 1} active=false />);

    let button = el |> getByText(~matcher=`Str("Click!"));
    button->FireEvent.click;

    c^ |> expect |> toBe(0);
  });
});

describe("Counter(Outer)", () => {
  test("render", () => {
    <CCounter />
    |> render
    |> expect
    |> toMatchSnapshot;
  });
});