let component = ReasonReact.statelessComponent("Essay");

let str = ReasonReact.string;

let make = (~title, ~text, _children) => {
  ...component,
  render: _self => {
    <div>
      <h1>(str(title))</h1>
      <p>(str(text))</p>
    </div>
  },
};
