module InlinePopover = {
  include ReactRe.Component;
  let name = "InlinePopover";
  type props = {
    message: string,
    close: unit => unit
  };
  let render {props} =>
    <div className="root-container">
      <Transition
        before=(ReactDOMRe.Style.make opacity::"0" ())
        after=(ReactDOMRe.Style.make opacity::"1" transition::"opacity 250ms" ())>
        <div className="mask-container" onClick=(fun _ => props.close ())>
          <div className="error-message-container">
            (ReactRe.stringToElement props.message)
          </div>
        </div>
      </Transition>
    </div>;
};

include ReactRe.CreateComponent InlinePopover;

let createElement ::message ::close =>
  wrapProps {message, close};