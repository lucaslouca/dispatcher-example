<template>
  <RouterView />
</template>

<script setup lang="ts">
import { isAuthenticated } from '@/api'
import { RouterView } from 'vue-router'
import { useRoute, useRouter } from 'vue-router'

console.log('[App] created')

const router = useRouter()
const route = useRoute()
const authenticated: boolean = isAuthenticated()
console.log('[App] authenticated=' + authenticated)
console.log('[App] route.fullPath=', route.name)
if (route.name === 'updatepassword') {
  console.log('[App] Redirect to updatepassword.')
  router.replace({
    name: 'updatepassword',
    query: { id: route.query.id, token: route.query.token }
  })
} else if (route.name === 'login') {
  console.log('[App] Redirect to login.')
  router.replace({ name: 'login' })
} else {
  var target = route.path
  console.log('[App] Do nothing.')
  if (target === '/') {
    target = 'members/dashboard'
  }
  // router.push({ path: target })
}
</script>

<style lang="scss">
* {
  font-family: 'Oxygen', sans-serif;
}

body {
  margin: 0 !important;
}
#app {
  height: 100vh;
}
</style>
