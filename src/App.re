type item = {
  title: string,
  completed: bool,
};

type state = {
  items: list(item),
};

type action =
  | AddItem;

let component = ReasonReact.reducerComponent("App");

let newItem = () => { title: "Click a button", completed: true };

let str = ReasonReact.string;

let make = _children => {
  ...component,
  initialState: () => {
    items: [
      {
        title: "Write some things to do!",
        completed: false,
      }
    ]
  },
  reducer: (action, { items }) =>
    switch action {
    | AddItem => ReasonReact.Update({ items: [ newItem(), ...items ] })
    },
  render: ({ state, send }) => {
    let numItems = List.length(state.items);
    <div>
      <p>
        (str("What to do?"))
      </p>
      <button
        onClick=(_evt => send(AddItem))
      >
        (str("Add something"))
      </button>
      <p>
        (str("Nothing."))
      </p>
      <footer>
        (str(string_of_int(numItems) ++ " items"))
      </footer>
    </div>
  },
};
