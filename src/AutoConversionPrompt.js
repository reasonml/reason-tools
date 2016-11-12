import convertOcamldoc from './convertOcamldoc';
import convertRealWorldOcaml from './convertRealWorldOcaml';

const scriptTag = document.createElement('script');

function hasClassName(className) {
  const els = document.getElementsByClassName(className);
  return els.length > 0;
}

const ocamlishRels = [
  'Start',
  'previous',
  'next',
  'Up',
  'Appendix',
  'Section',
  'Subsection',
];

function hasOcamlRels() {
  return Array.prototype.map.call(
    document.getElementsByTagName('link'),
    (link) => ocamlishRels.indexOf(link.getAttribute('rel')) > -1,
  ).filter(Boolean).length >= 3;
}

function hasCommonClassNames() {
  return [
    'keyword',
    'type',
    'deprecated',
    'mod',
    'modt',
    'typ',
    'spec',
    'def',
    'val',
    'ext',
    'exn',
    'cls',
    'clst',
    'include',
    'cons',
    'paramstable',
    'sig_block',
  ].map(hasClassName).filter(Boolean).length >= 3;
}

function hasUniqueClassNames() {
  return [
    'odoc-doc',
    'package-index',
  ].map(hasClassName).filter(Boolean).length > 0;
}

function mightBeOcamlDoc() {
  return hasUniqueClassNames() || hasOcamlRels() || hasCommonClassNames();
}

function isRealWorldOcaml() {
  return window.location.origin === 'https://realworldocaml.org';
}

if (isRealWorldOcaml()) {
  convertRealWorldOcaml();
}else if (mightBeOcamlDoc()) {
  convertOcamldoc();
}
