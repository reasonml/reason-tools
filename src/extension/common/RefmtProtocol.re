open Core;

exception DeserializationFail;

type request = {
  input: string
};

type payload = {
  outText: string,
  inLang: string,
  outLang: string
};

type response =
| Success payload
| Failure string;

/* Bucklescript's variant tags will be erased when serialized, so we have to manually serialize the response
 */
let serialize = fun
| Failure error => (0, { outText: error, inLang: "", outLang: ""})
| Success payload => (1, payload);

let deserialize = fun
| (0, { outText: error }) => Failure error
| (1, payload) => Success payload
| _ => raise DeserializationFail;

let send : request => (response => unit) => unit =
  fun request cb => Chrome.Runtime.sendMessage request (fun response => cb (deserialize response ));
let listen : (request => (response => unit) => unit) => unit =
  fun cb => Chrome.Runtime.addMessageListener (fun request _ respond => cb request (fun r => r |> serialize |> respond));
