module Transition = {
  include ReactRe.Component.Stateful;
  let name = "Transition";
  type props = {
    before: ReactDOMRe.style,
    after: ReactDOMRe.style,
    children: list ReactRe.reactElement
  };
  type state = {style: ReactDOMRe.style};
  let getInitialState props => {style: props.before};
  let componentDidMount {updater} => {
    Core.Util.setTimeout (updater (fun {props} _ => Some {style: props.after})) 0;
    None
  };
  let render {props, state} =>
    <div style=state.style> (ReactRe.arrayToElement (Array.of_list props.children)) </div>;
};

include ReactRe.CreateComponent Transition;

let createElement ::before ::after ::children => wrapProps {before, after, children} ::children;
