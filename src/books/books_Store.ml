module Store = Store.Make(Books_Action)(Books_State)

include Store

let () = React.setDisplayName make "BooksStore"