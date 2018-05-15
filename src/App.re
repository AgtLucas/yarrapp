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

type routes =
  | Home
  | Essay
  | NotFound;

type action =
  | UpdateTitle(string)
  | UpdateDescription(string)
  | ChangeRoutes(routes)
  | Create;

type article = {
  title: string,
  description: string,
};

type state = {
  /* article: list(article), */
  title: string,
  description: string,
  currentPage: routes,
};

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["essay"] => Essay
  | _ => NotFound
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
    currentPage: ReasonReact.Router.dangerouslyGetInitialUrl() |> mapUrlToRoute,
  },
  reducer: (action, state) =>
    switch (action) {
    | UpdateTitle(value) => ReasonReact.Update({ ...state, title: value })
    | UpdateDescription(value) => ReasonReact.Update({ ...state, description: value })
    | ChangeRoutes(nextRoute) => ReasonReact.Update({ ...state, currentPage: nextRoute })
    | Create => ReasonReact.UpdateWithSideEffects(
      { ...state, title: state.title, description: state.description },
      (_self => {
        saveLocally({ ...state, title: state.title, description: state.description });
        ReasonReact.Router.push("essay");
      })
    )
    },
  didMount: self => {
    let watcherID =
      ReasonReact.Router.watchUrl(url => {
        self.send(ChangeRoutes(url |> mapUrlToRoute))
      });
      self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },
  render: ({ state: { title, description, currentPage }, send }) => {
    switch (currentPage) {
    | Home => {
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
      </div>
    }
    | Essay => <Essay title description />
    | NotFound => <NotFound />
    }
  },
};
