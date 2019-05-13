
[@react.component]
let make = (~toggle, ~content) => {
  let (isShow, setIsShow) = React.useState(() => false);
  <>
    {toggle(_ => setIsShow(_ => true))}
    {isShow ? content(_ => setIsShow(_ => false)) : React.null}
  </>
}