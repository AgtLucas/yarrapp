open Belt;

[@bs.val] external unsafeJsonParse : string => 'a = "JSON.parse";

let localStorageNamespace = "yarrapp_";

let saveLocally = article =>
  switch (Js.Json.stringifyAny(article)) {
  | None => ()
  | Some(stringifiedArticle) =>
    Dom.Storage.(
      localStorage |> setItem(localStorageNamespace, stringifiedArticle)
    )
  };

type action =
  | UpdateTitle(string)
  | UpdateDescription(string)
  | Create;

type article = {
  title: string,
  description: string,
};

type state = {
  article: list(article),
};

let component = ReasonReact.reducerComponent("App");

let str = ReasonReact.string;

let valueFromEvent = (evt) : string => (
  evt
  |> ReactEventRe.Form.target
  |> ReactDOMRe.domElementToObj
)##value;

let updateTitle = (event) => UpdateTitle(valueFromEvent(event));

let updateDescription = (event) => UpdateDescription(valueFromEvent(event));

let make = _children => {
  ...component,
  initialState: () => {
    title: "",
    description: "",
  },
  reducer: (action, state) =>
    switch (action) {
    | UpdateTitle(value) => ReasonReact.Update({ ...state, title: value })
    | UpdateDescription(value) => ReasonReact.Update({ ...state, description: value })
    | Create => ReasonReact.UpdateWithSideEffects(
      { title: state.title, description: state.description },
      (_self => saveLocally({ title: state.title, description: state.description }))
    )
    },
  render: ({ state: { title, description }, send }) => {
    <div>
      <form
        onSubmit=((_) => send(Create))
      >
        <label>
          (str("Title"))
          <input
            _type="text"
            value=title
            placeholder="e.g.: My awesome title!"
            onChange=(event => send(updateTitle(event)))
          />
        </label>
        <label>
          (str("Description"))
          <input
            _type="text"
            value=description
            placeholder="e.g.: This is an awesome description, because, well, why not?"
            onChange=(event => send(updateDescription(event)))
          />
        </label>
        <button>
          (str("Create"))
        </button>
      </form>
      <footer>
        <p>
          (str(title))
        </p>
        <p>
          (str(description))
        </p>
      </footer>
    </div>
  },
};
