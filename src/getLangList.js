
export default function(conversionType) {
  return conversionType && conversionType !== 'Failure'
    ? conversionType.split('to')
    : [null, null];
}
