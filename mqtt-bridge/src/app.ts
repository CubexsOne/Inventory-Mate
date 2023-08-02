import { MQTTClient } from './mqtt'
import { environment } from './utils/environment'
import logger from './utils/logger'

function runApplication(): void {
  logger.info('MQTT-Bridge is starting...')
  new MQTTClient({
    host: environment.MQTT_HOST,
    port: environment.MQTT_PORT,
    username: environment.MQTT_USERNAME,
    password: environment.MQTT_PASSWORD,
    readTopic: environment.MQTT_READ_TOPIC,
    writeTopic: environment.MQTT_WRITE_TOPIC,
    callback: mockRequest,
  })
}

async function mockRequest(text: string): Promise<string> {
  return 'Red Bull White Edition'
}

runApplication()
