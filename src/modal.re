module Modal = Modals_Modal;

module SampleModal = {
  [@react.component]
  let make = () => {
    <Modal
      toggle={show => {
        <button className="button" onClick={show}>
          {React.string("Open Modal")}
        </button>
      }}
      content={hide => {
        let onClick = event => {
          event->ReactEvent.Mouse.preventDefault;
          hide();
        };
        <div className="modal is-active">
          <div className="modal-background"></div>
          <div className="modal-card">
            <header className="modal-card-head">
              <p className="modal-card-title">{React.string("Modal Sample")}</p>
              <button className="delete" ariaLabel="close" onClick />
            </header>
            <section className="modal-card-body">
              <p>{React.string("This is a sample Modal.")}</p>
            </section>
            <footer className="modal-card-foot">
              <button className="button" onClick>{React.string("Close")}</button>
            </footer>
          </div>
        </div>
      }}
    />
  }
};

[@react.component]
let make = () => {
  <>
    <SampleModal />
    <br />
    <Link href="/">{React.string("Back")}</Link>
  </>
}