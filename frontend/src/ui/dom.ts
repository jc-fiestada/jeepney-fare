export function requireElement<T extends HTMLElement>(id: string): T {
  const element = document.getElementById(id);
  if (element === null) {
    throw new Error(`Element #${id} was not found in the page.`);
  }
  return element as T;
}