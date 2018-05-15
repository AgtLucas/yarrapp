let component = ReasonReact.statelessComponent("NotFound");

let str = ReasonReact.string;

let make = _children => {
  ...component,
  render: _self => <h1>(str("Oh, no!"))</h1>,
};
