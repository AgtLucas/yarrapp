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
  | UpdateText(string)
  | ChangeRoutes(routes)
  | Create;

type article = {
  title: string,
  text: string,
};

type state = {
  /* article: list(article), */
  title: string,
  text: string,
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

let updateText = (event) => UpdateText(valueFromEvent(event));

let make = _children => {
  ...component,
  initialState: () => {
    title: "",
    text: "",
    currentPage: ReasonReact.Router.dangerouslyGetInitialUrl() |> mapUrlToRoute,
  },
  reducer: (action, state) =>
    switch (action) {
    | UpdateTitle(value) => ReasonReact.Update({ ...state, title: value })
    | UpdateText(value) => ReasonReact.Update({ ...state, text: value })
    | ChangeRoutes(nextRoute) => ReasonReact.Update({ ...state, currentPage: nextRoute })
    | Create => ReasonReact.UpdateWithSideEffects(
      { ...state, title: state.title, text: state.text },
      (_self => {
        saveLocally({ ...state, title: state.title, text: state.text });
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
  render: ({ state: { title, text, currentPage }, send }) => {
    switch (currentPage) {
    | Home => {
      <Form
        onChangeTitle=(event => send(updateTitle(event)))
        onChangeText=(event => send(updateText(event)))
        createEssay=(_ => send(Create))
        title
        text
      />
    }
    | Essay => <Essay title text />
    | NotFound => <NotFound />
    }
  },
};
