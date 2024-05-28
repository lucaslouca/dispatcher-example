import axios from 'axios'
import type { AxiosResponse } from 'axios'

const ENDPOINT_STRATEGY: string = 'strategy'

type Handler<T = unknown> = (event: T) => Promise<void | AxiosResponse<any, any>>

export function isAuthenticated() {
  const token = localStorage.getItem('user')
  if (token) {
    const user = JSON.parse(token)
    console.log('[Api] Checking Authentication...')
    if (user && user.token) {
      console.log('[Api] User is authenticated.')
      return true
    } else {
      console.log('[Api] User is not authenticated.')
      return false
    }
  }
  return false
}

export function authHeader() {
  // return authorization header with jwt token
  const token = localStorage.getItem('user')
  if (token) {
    const user = JSON.parse(token)
    if (user && user.token) {
      const header = { Authorization: 'Bearer ' + user.token }
      return header
    } else {
      return {}
    }
  } else {
    return {}
  }
}

export class AppAPI {
  #url: string
  #endpoints = new Map<string, Map<string, Handler<any>>>()

  constructor(url: string) {
    this.#url = url
  }

  /**
   * Create and store a single entity's endpoints
   * @param {A entity Object} entity
   */
  createEntity(entity: { name: string }) {
    this.#endpoints.set(entity.name, this.createBasicCRUDEndpoints(entity))
  }

  /**
   * Create the basic endpoints handlers for CRUD operations
   * @param {A entity Object} entity
   */
  createBasicCRUDEndpoints(entity: { name: string }) {
    const handlers =
      /** @type {Map<string, Handler<any>>} */ this.#endpoints.get(entity.name) ??
      new Map<string, Handler<any>>()

    const resourceURL = `${this.#url}/${entity.name}`

    // const resourceURL = `${this.#url}`
    // handlers.set('getById', (id: any, config = { headers: authHeader() }) =>
    //   axios.get(`${resourceURL}/api/v1/resource`, config).catch(function (error: any) {
    //     console.log(`[Api.js - getById - ${resourceURL}] Error: `, error.response)
    //   })
    // )

    handlers.set('getAll', (id: any, config = { headers: authHeader() }) =>
      axios.get(`${resourceURL}/api/v1/resource`, config).catch(function (error: any) {
        console.log(`[Api.js - getById - ${resourceURL}] Error: `, error.response)
      })
    )

    handlers.set('runStrategy', (credentials, config = { headers: authHeader() }) =>
      axios.post(`${resourceURL}/strategy`, credentials, config)
    )

    return handlers
  }

  getEndpoints(name: string): Map<string, Handler<any>> | undefined {
    return this.#endpoints.get(name)
  }
}

// let API = new AppAPI(process.env.VUE_APP_API_URL)
const API = new AppAPI('http://127.0.0.1:8080')
API.createEntity({ name: ENDPOINT_STRATEGY })

export { API }
