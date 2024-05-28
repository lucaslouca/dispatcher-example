export type State = {
  id: string
  name: string
  audio: boolean
  camera?: string
}
/**
 * Load app state from local storage
 */
export function loadState(stateKey: string): State {
  let stateStr = localStorage.getItem(stateKey)
  if (!stateStr) {
    const state = {
      name: 'Anonymous',
      audio: true
    }
    stateStr = JSON.stringify(state)
    localStorage.setItem(stateKey, stateStr)
  }
  return JSON.parse(stateStr) as State
}

/**
 * Return an element, which must exist.
 */
export function getElement<T extends HTMLElement>(selector: string): T {
  const elem = document.querySelector(selector) as T
  if (!elem) {
    throw new Error(`Element not found: ${selector}`)
  }
  return elem
}

export interface Dictionary<T> {
  [Key: string]: T
}

export class SearchParameters {
  SearchFor: Dictionary<string> = {}
}
