import logger from './logger'

export const environment = {
  NODE_ENV: process.env.NODE_ENV || 'DEV',
  MQTT_HOST: process.env.MQTT_HOST || 'localhost',
  MQTT_PORT: parseToNumber(process.env.MQTT_HOST || '1883'),
  MQTT_USERNAME: process.env.MQTT_HOST || 'USER',
  MQTT_PASSWORD: process.env.MQTT_HOST || 'PASSWORD',
  MQTT_READ_TOPIC: process.env.MQTT_HOST || 'READ',
  MQTT_WRITE_TOPIC: process.env.MQTT_HOST || 'WRITE',
}

function parseToNumber(value: string | undefined): number {
  if (!value) {
    return 0
  }
  try {
    const parsedNumber = parseInt(value)
    if (parsedNumber.toString().length !== value.length) {
      throw new Error(`Value "${value}" parsed not successfully!`)
    }
    return parsedNumber
  } catch (error) {
    logger.error('parseToNumber:', error)
    return 0
  }
}
