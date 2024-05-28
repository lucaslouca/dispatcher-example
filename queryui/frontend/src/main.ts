import './assets/main.css'

import { createApp } from 'vue'
import axios from 'axios'
import App from './App.vue'
import router from './router'

import { library } from '@fortawesome/fontawesome-svg-core'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'
import { fas } from '@fortawesome/free-solid-svg-icons'
import { fab } from '@fortawesome/free-brands-svg-icons'
import { far } from '@fortawesome/free-regular-svg-icons'
library.add(fas, far, fab)

const app = createApp(App).component('font-awesome-icon', FontAwesomeIcon)

// router.beforeEach((to, from, next) => {
//   console.log(to)
//   //   document.title = process.env.VUE_APP_NAME + ' - ' + to.name[0].toUpperCase() + to.name.slice(1)
//   next()
// })

axios.interceptors.response.use(
  function (response) {
    return response
  },
  function (error) {
    if (error.response.status === 401 || error.response.status === 403) {
      router.push('/login').catch((err) => {
        // Ignore the vuex err regarding  navigating to the page they are already on.
        if (
          err.name !== 'NavigationDuplicated' &&
          !err.message.includes('Avoided redundant navigation to current location')
        ) {
          // But print any other errors to the console
          console.log(err)
        }
      })
    }
    return Promise.reject(error)
  }
)

app.use(router)
app.mount('#app')
