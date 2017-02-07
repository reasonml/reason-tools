open Core;

module Refmt = {
  external refmt : string => (string, string) = "refmt" [@@bs.module "../../../../_build/refmt/src/app.js"];
};

type request = {
  in_: string
};

type response = {
  out: (string, string)
};

Chrome.Runtime.addMessageListener
	(fun request _ respond =>
     	respond { out: (Refmt.refmt request.in_) });
